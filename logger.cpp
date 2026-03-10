//
// Created by uid248 on 10.03.2026.
//

#include "logger.h"
#include <QFileDialog>

#include <QJsonObject>
#include <QJsonArray>


void Logger::generateLog(QString repoUrl, QString token, QString destination, int page)
{
    qDebug() << "URL: " << repoUrl;
    qDebug() << "Token: " << token;
    qDebug() << "Destination: " << destination;

    // QUrl url(repoUrl + "/api/v4/projects/" + projectId + "/repository/commits");
    // QUrl url("https://gitlab.veratron.com/api/v4/projects/32/repository/commits?per_page=100&page=" + QString::number(page));
    QUrl url(repoUrl);

    QString host = url.host();
    QString scheme = url.scheme(); // http oder https
    QString path = url.path();

    if (path.endsWith(".git")) {
        path.chop(4);
    }

    if (path.startsWith("/")) path.remove(0, 1);
    QString encodedPath = QUrl::toPercentEncoding(path);

    // QString finalUrl = QString("%1://%2/api/v4/projects/%3/repository/commits").arg(scheme).arg(host).arg(path);
    QUrl finalUrl = QString(scheme + "://" + host + "/api/v4/projects/" + encodedPath + "/repository/commits?per_page=100&page=" + QString::number(page));
    // QUrl finalUrl(QString("%1://%2/api/v4/projects/%3/repository/commits?per_page=100&page=%4").arg(scheme).arg(host).arg(encodedPath).arg(QString::number(page)));
    qInfo() << finalUrl;

    QNetworkRequest request(finalUrl);

    // Authentication via Header
    request.setRawHeader("PRIVATE-TOKEN", token.toUtf8());

    // Send request
    QNetworkReply *reply = manager->get(request);

    // Process the answer when it has been received
    connect(reply, &QNetworkReply::finished, this, [this, reply, repoUrl, token, destination, page, repoUrl]() {
        if (reply->error() == QNetworkReply::NoError) {
            // 1. Parse data
            QByteArray jsonData = reply->readAll();


            // 2. Check if there is another commit page
            int totalPages = reply->rawHeader("X-Total-Pages").toInt();

            if (page < totalPages) {
                // Recursive call for the next page
                generateLog(repoUrl, token, destination, page + 1);
            } else {
                qDebug() << "All commits received!";

                QJsonArray jsonArray = parseJson(jsonData);
                // this->savePrettyJson(jsonArray, destination);
                this->saveToCsv(jsonArray, destination);
            }
        }
        reply->deleteLater();
    });
}

QJsonArray Logger::parseJson(const QByteArray &data) {
    QJsonDocument doc = QJsonDocument::fromJson(data);

    // GitLab Commits come as Array [ {...}, {...} ]
    if (!doc.isArray()) {
        qDebug() << "Error: Expected JSON-Array, but contained something else.";
        return QJsonArray();
    }

    QJsonArray commits = doc.array();
    for (const QJsonValue &value : commits) {
        QJsonObject obj = value.toObject();

        // Errors, which Gitlab delivered:
        QString shortId = obj["short_id"].toString();
        QString title   = obj["title"].toString();
        QString message = obj["message"].toString();
        QString author  = obj["author_name"].toString();
        QString date    = obj["created_at"].toString();

        qDebug() << "Found:" << shortId << "|" << author << ":" << message;
    }
    return commits;
}

void Logger::savePrettyJson(const QJsonArray &jsonArray, QString filePath) {
    QJsonDocument doc(jsonArray);

    QString now = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");

    QFile file(filePath + "/gitlab_log_" + now + ".json");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
    }
}

void Logger::saveToCsv(const QJsonArray &jsonArray, QString filePath) {

    QString now = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");

    QFile file(filePath + "/gitlab_log_" + now + ".csv");
    // Wichtig: UTF-8 BOM für Excel-Kompatibilität aktivieren
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Fehler beim Öffnen der Datei!";
        return;
    }

    QTextStream out(&file);
    out.setGenerateByteOrderMark(true); // Verhindert "Hieroglyphen" bei Umlauten in Excel

    // 1. Spaltenüberschriften (Passe die Attribute hier an)
    out << "Short ID;Author;Date;Title;Message\n";

    // 2. Das Array durchlaufen
    for (const QJsonValue &value : jsonArray) {
        QJsonObject obj = value.toObject();

        // Attribute aus dem JSON extrahieren
        QString id      = obj["short_id"].toString();
        QString author  = obj["author_name"].toString();
        QString date    = obj["created_at"].toString();
        QString title   = obj["title"].toString();
        QString message = obj["message"].toString();

        // 3. Daten "säubern" (WICHTIG für CSV)
        // Entfernt Zeilenumbrüche und ersetzt Semikolons, damit die Spalten nicht verrutschen
        title = title.simplified().replace(";", ",");
        message = message.simplified().replace(";", ",");
        author  = author.simplified().replace(";", ",");

        // 4. In die Datei schreiben
        out << id << ";"
            << author << ";"
            << date << ";"
            << title << ";"
            << message << "\n";
    }

    file.close();
    qDebug() << "CSV erfolgreich gespeichert unter:" << file.fileName();
}