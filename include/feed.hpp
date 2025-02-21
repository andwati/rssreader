//
// Created by ian on 2/22/25.
//
#pragma once
#include <QString>
#include <vector>

struct FeedItem {
    QString title;
    QString link;
    QString description;
    QString pubDate;
};

class RSSFeed {
public:
    QString feedUrl;
    QString feedTitle;
    std::vector<FeedItem> items;
};
