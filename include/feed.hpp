//
// Created by ian on 2/22/25.
//
#pragma once
#include "feed.hpp"

#include <QString>
#include <vector>
#include <QDateTime>
#include <QUuid>

struct FeedItem {
  QString title;
  QString link;
  QString description;
  QString pubDate;
  QDateTime published;
  bool isRead{false};
  QUuid id{QUuid::createUuid()};
};

class RSSFeed {
public:
  QString feedUrl;
  QString feedTitle;
  QString description;
  QString category;
  std::vector<FeedItem> items;
  QDateTime lastUpdated;
  bool autoRefresh{true};
  int refreshInterval{30}; // in minutes
  int displayOrder{0};
  QUuid id{QUuid::createUuid()};

  // format settings
  struct FormatSettings {
    QString titleFont{"Arial"};
    int titleSize{16};
    QString contentFont{"Arial"};
    int contentSize{12};
    QString backgroundColor{"#FFFFFF"};
    QString textColor{"#000000"};
    QString linkColor{"#0066CC"};
    bool showImages{true};
    bool useCustomCSS{false};
    QString customCSS;
  } formatting;
};
