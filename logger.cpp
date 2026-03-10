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

    QUrl url(repoUrl);

    QString host = url.host();
    QString scheme = url.scheme(); // http oder https
    QString path = url.path();

    if (path.endsWith(".git")) {
        path.chop(4);
    }

    if (path.startsWith("/")) path.remove(0, 1);
    QString encodedPath = QUrl::toPercentEncoding(path);

    QUrl finalUrl = QString(scheme + "://" + host + "/api/v4/projects/" + encodedPath + "/repository/commits?per_page=100&page=" + QString::number(page));
    QNetworkRequest request(finalUrl);

    // Authentication via Header
    request.setRawHeader("PRIVATE-TOKEN", token.toUtf8());

    // Send request
    QNetworkReply *reply = manager->get(request);

    // Process the answer when it has been received
    connect(reply, &QNetworkReply::finished, this, [this, reply, repoUrl, token, destination, page, repoUrl]() {
        if (reply->error() == QNetworkReply::NoError) {
            // Parse data
            QByteArray jsonData = reply->readAll();

            // Check if there is another commit page
            int totalPages = reply->rawHeader("X-Total-Pages").toInt();

            if (page < totalPages) {
                // Recursive call for the next page
                generateLog(repoUrl, token, destination, page + 1);
            } else {
                qDebug() << "All commits received!";

                QJsonArray jsonArray = parseJson(jsonData);
                if (destination.contains(".csv")) {
                    this->saveToCsv(jsonArray, destination);
                }
                else {
                    this->savePrettyJson(jsonArray, destination);
                }
            }
        }
        reply->deleteLater();
    });
}

QJsonArray Logger::parseJson(const QByteArray &data) {
    QJsonDocument doc = QJsonDocument::fromJson(data);

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

    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
    }
}

void Logger::saveToCsv(const QJsonArray &jsonArray, QString filePath) {
    QString now = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");

    QFile file(filePath);
    // UTF-8 BOM activation for Excel compatibility
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Error opening the file!";
        return;
    }

    QTextStream out(&file);
    out.setGenerateByteOrderMark(true); // Suppress issues in special letters (ä, ö, ü)

    // Title row
    out << "Short ID;Author;Date;Title;Message\n";

    for (const QJsonValue &value : jsonArray) {
        QJsonObject obj = value.toObject();

        QString id      = obj["short_id"].toString();
        QString author  = obj["author_name"].toString();
        QString date    = obj["created_at"].toString();
        QString title   = obj["title"].toString();
        QString message = obj["message"].toString();

        title = title.simplified().replace(";", ",");
        message = message.simplified().replace(";", ",");
        author  = author.simplified().replace(";", ",");

        out << id << ";"
            << author << ";"
            << date << ";"
            << title << ";"
            << message << "\n";
    }

    file.close();
}