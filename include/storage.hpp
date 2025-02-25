//
// Created by ian on 2/23/25.
//

#pragma once
#include "feed.hpp"
#include <QObject>
#include <memory>
#include <vector>

class FeedStorage : public QObject {
  Q_OBJECT

public:
  static FeedStorage &instance();

  static bool saveFeeds(const std::vector<std::unique_ptr<RSSFeed>> &feeds);
  static bool loadFeeds(std::vector<std::unique_ptr<RSSFeed>> &feeds) ;

  static bool saveCategories(const std::vector<FeedCategory> &categories);
  static bool loadCategories(std::vector<FeedCategory> &categories);

private:
  FeedStorage();
  static QString getStoragePath();
};