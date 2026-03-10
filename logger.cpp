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
    QUrl url("https://gitlab.veratron.com/api/v4/projects/32/repository/commits?per_page=100&page=" + QString::number(page));
    QNetworkRequest request(url);

    // Authentication via Header
    request.setRawHeader("PRIVATE-TOKEN", token.toUtf8());

    // Send request
    QNetworkReply *reply = manager->get(request);

    // Process the answer when it has been received
    connect(reply, &QNetworkReply::finished, this, [this, reply, repoUrl, token, destination, page]() {
        if (reply->error() == QNetworkReply::NoError) {
            // 1. Parse data
            parseJson(reply->readAll());

            // 2. Check if there is another commit page
            int totalPages = reply->rawHeader("X-Total-Pages").toInt();

            if (page < totalPages) {
                // Recursive call for the next page
                generateLog(repoUrl, token, destination, page + 1);
            } else {
                qDebug() << "All commits received!";
            }
        }
        reply->deleteLater();
    });
}

void Logger::parseJson(const QByteArray &data) {
    QJsonDocument doc = QJsonDocument::fromJson(data);

    // GitLab Commits come as Array [ {...}, {...} ]
    if (!doc.isArray()) {
        qDebug() << "Error: Expected JSON-Array, but contained something else.";
        return;
    }

    QJsonArray commits = doc.array();
    for (const QJsonValue &value : commits) {
        QJsonObject obj = value.toObject();

        // Errors, which Gitlab delivered:
        QString shortId = obj["short_id"].toString();
        QString message = obj["title"].toString();
        QString author  = obj["author_name"].toString();
        QString date    = obj["created_at"].toString();

        qDebug() << "Found:" << shortId << "|" << author << ":" << message;
    }
}