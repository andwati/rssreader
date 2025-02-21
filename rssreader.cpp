//
// Created by ian on 2/22/25.
//
#include "rssreader.h"
#include <QHBoxLayout>
#include <QNetworkRequest>
#include <QXmlStreamReader>

RSSReader::RSSReader(QWidget *parent) : QWidget(parent), networkManager(new QNetworkAccessManager(this)) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    urlInput = new QLineEdit(this);
    fetchButton = new QPushButton("Fetch RSS", this);
    listWidget = new QListWidget(this);

    layout->addWidget(urlInput);
    layout->addWidget(fetchButton);
    layout->addWidget(listWidget);
    setLayout(layout);

    connect(fetchButton, &QPushButton::clicked, this, &RSSReader::fetchRSS);
    connect(listWidget, &QListWidget::itemClicked, this, &RSSReader::openArticle);
    connect(networkManager, &QNetworkAccessManager::finished, this, &RSSReader::onReplyFinished);
}

void RSSReader::fetchRSS() {
    QUrl url(urlInput->text());
    QNetworkRequest request(url);
    networkManager->get(request);
}

void RSSReader::onReplyFinished(QNetworkReply* reply) {
    if (reply->error() != QNetworkReply::NoError) {
        listWidget->addItem("Error fetching RSS feed");
        reply->deleteLater();
        return;
    }

    QXmlStreamReader xml(reply);
    QString title, link;

    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        if (xml.isStartElement()) {
            if (xml.name() == "item") {
                title.clear();
                link.clear();
            } else if (xml.name() == "title") {
                title = xml.readElementText();
            } else if (xml.name() == "link") {
                link = xml.readElementText();
            }
        } else if (xml.isEndElement() && xml.name() == "item") {
            if (!title.isEmpty() && !link.isEmpty()) {
                QListWidgetItem *item = new QListWidgetItem(title, listWidget);
                item->setData(Qt::UserRole, link);
            }
        }
    }
    reply->deleteLater();
}

void RSSReader::openArticle(QListWidgetItem* item) {
    QString url = item->data(Qt::UserRole).toString();
    QDesktopServices::openUrl(QUrl(url));
}

