//
// Created by uid248 on 10.03.2026.
//

#include "logger.h"
#include <QFileDialog>
#include <QJsonObject>


void Logger::getRepoTags(QString repoUrl, QString token, QString destination) {
    qDebug() << "URL: " << repoUrl;
    qDebug() << "Token: " << token;

    QUrl url(repoUrl);

    QString host = url.host();
    QString scheme = url.scheme(); // http oder https
    QString path = url.path();

    if (path.endsWith(".git")) {
        path.chop(4);
    }

    if (path.startsWith("/")) path.remove(0, 1);
    QString encodedPath = QUrl::toPercentEncoding(path);

    QUrl finalUrl = QString(scheme + "://" + host + "/api/v4/projects/" + encodedPath + "/repository/tags?per_page=100");
    QNetworkRequest request(finalUrl);

    // Authentication via Header
    request.setRawHeader("PRIVATE-TOKEN", token.toUtf8());

    // Send request
    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply, repoUrl, token, destination]() {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            QJsonArray tags = doc.array();

            for (const QJsonValue &v : tags) {
                QJsonObject obj = v.toObject();
                QString tagName = obj["name"].toString();
                QString commitSha = obj["commit"].toObject()["id"].toString();
                tagMap.insert(commitSha, tagName);
            }
            qDebug() << tagMap.size() << " Tags loaded.";

            generateLog(repoUrl, token, destination, 1);
        }
        reply->deleteLater();
    });
}

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
    connect(reply, &QNetworkReply::finished, this, [this, reply, repoUrl, token, destination, page]() {
        if (reply->error() == QNetworkReply::NoError) {
            // Parse data
            QByteArray jsonData = reply->readAll();
            QJsonArray jsonArray = parseJson(jsonData);
            for (const QJsonValue &value : jsonArray) {
                commitCollection.append(value);
            }

            // Check if there is another commit page
            int nextPage = reply->rawHeader("x-next-page").toInt();

            if (nextPage > page) {
                // Recursive call for the next page
                generateLog(repoUrl, token, destination, nextPage);
            } else {
                qDebug() << "All commits received!";
                qDebug() << "Number of commits: " << commitCollection.size();

                if (destination.contains(".csv")) {
                    this->saveToCsv(commitCollection, destination);
                }
                else {
                    this->savePrettyJson(commitCollection, destination);
                }

                commitCollection = QJsonArray();
                emit workFinished();
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
    out << "Short ID;Version;Author;Date;Title;Message\n";

    QString version = "V000";

    for (const QJsonValue &value : jsonArray) {
        QJsonObject obj = value.toObject();

        QString id      = obj["short_id"].toString();
        QString id_long = obj["id"].toString();
        QString author  = obj["author_name"].toString();
        QString date    = obj["created_at"].toString();
        QString title   = obj["title"].toString();
        QString message = obj["message"].toString();

        QString temp_version = tagMap.value(id_long, "");
        if (!comparesEqual(temp_version, "")) {
            version = temp_version;
        }

        title = title.simplified().replace(";", ",");
        message = message.simplified().replace(";", ",");
        author  = author.simplified().replace(";", ",");

        out << id << ";"
            << version << ";"
            << author << ";"
            << date << ";"
            << title << ";"
            << message << "\n";
    }

    file.close();
}