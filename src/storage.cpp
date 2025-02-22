#include "storage.hpp"
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

FeedStorage& FeedStorage::instance() {
    static FeedStorage instance;
    return instance;
}

FeedStorage::FeedStorage() {
    QDir dir(getStoragePath());
    if (!dir.exists()) {
        dir.mkpath(".");
    }
}

QString FeedStorage::getStoragePath() const {
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
}

bool FeedStorage::saveFeeds(const std::vector<std::unique_ptr<RSSFeed>>& feeds) {
    QJsonArray feedsArray;

    for (const auto& feed : feeds) {
        QJsonObject feedObj;
        feedObj["url"] = feed->feedUrl;
        feedObj["title"] = feed->feedTitle;
        feedObj["category"] = feed->category;
        feedObj["autoRefresh"] = feed->autoRefresh;
        feedObj["refreshInterval"] = feed->refreshInterval;
        feedObj["lastUpdated"] = feed->lastUpdated.toString(Qt::ISODate);

        QJsonArray itemsArray;
        for (const auto& item : feed->items) {
            QJsonObject itemObj;
            itemObj["title"] = item.title;
            itemObj["link"] = item.link;
            itemObj["description"] = item.description;
            itemObj["pubDate"] = item.pubDate;
            itemObj["isRead"] = item.isRead;
            itemsArray.append(itemObj);
        }
        feedObj["items"] = itemsArray;
        feedsArray.append(feedObj);
    }

    QFile file(getStoragePath() + "/feeds.json");
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    file.write(QJsonDocument(feedsArray).toJson());
    return true;
}

bool FeedStorage::loadFeeds(std::vector<std::unique_ptr<RSSFeed>>& feeds) {
    QFile file(getStoragePath() + "/feeds.json");
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isArray()) {
        return false;
    }

    feeds.clear();
    for (const auto& feedValue : doc.array()) {
        QJsonObject feedObj = feedValue.toObject();
        auto feed = std::make_unique<RSSFeed>();

        feed->feedUrl = feedObj["url"].toString();
        feed->feedTitle = feedObj["title"].toString();
        feed->category = feedObj["category"].toString();
        feed->autoRefresh = feedObj["autoRefresh"].toBool(true);
        feed->refreshInterval = feedObj["refreshInterval"].toInt(30);
        feed->lastUpdated = QDateTime::fromString(feedObj["lastUpdated"].toString(), Qt::ISODate);

        for (const auto& itemValue : feedObj["items"].toArray()) {
            QJsonObject itemObj = itemValue.toObject();
            FeedItem item;
            item.title = itemObj["title"].toString();
            item.link = itemObj["link"].toString();
            item.description = itemObj["description"].toString();
            item.pubDate = itemObj["pubDate"].toString();
            item.isRead = itemObj["isRead"].toBool();
            feed->items.push_back(item);
        }

        feeds.push_back(std::move(feed));
    }
    return true;
}

bool FeedStorage::saveCategories(const std::vector<FeedCategory>& categories) {
    QJsonArray categoriesArray;

    for (const auto& category : categories) {
        QJsonObject categoryObj;
        categoryObj["name"] = category.name;
        categoryObj["description"] = category.description;
        categoriesArray.append(categoryObj);
    }

    QFile file(getStoragePath() + "/categories.json");
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    file.write(QJsonDocument(categoriesArray).toJson());
    return true;
}

bool FeedStorage::loadCategories(std::vector<FeedCategory>& categories) {
    QFile file(getStoragePath() + "/categories.json");
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isArray()) {
        return false;
    }

    categories.clear();
    for (const auto& categoryValue : doc.array()) {
        QJsonObject categoryObj = categoryValue.toObject();
        FeedCategory category;
        category.name = categoryObj["name"].toString();
        category.description = categoryObj["description"].toString();
        categories.push_back(category);
    }
    return true;
}