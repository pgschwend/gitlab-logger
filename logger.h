//
// Created by uid248 on 10.03.2026.
//

#ifndef GITLAB_LOGGER_LOGGER_H
#define GITLAB_LOGGER_LOGGER_H

#include <QtNetwork/QNetworkReply>
#include <QJsonArray>


class Logger : public QObject {
private:
    QNetworkAccessManager *manager;
    QJsonArray parseJson(const QByteArray &data);
    void savePrettyJson(const QJsonArray &jsonArray, QString filePath);
    void saveToCsv(const QJsonArray &jsonArray, QString filePath);
    QMap<QString, QString> tagMap;
    QJsonArray commitCollection;

public:
    explicit Logger(QObject *parent = nullptr) : QObject(parent) {
        manager = new QNetworkAccessManager(this);
    }

    void generateLog(QString repoUrl, QString token, QString destination, int page);
    void getRepoTags(QString repoUrl, QString token, QString destination);
};


#endif //GITLAB_LOGGER_LOGGER_H