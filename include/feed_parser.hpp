//
// Created by ian on 2/22/25.
//

#pragma once
#include "feed.hpp"

class FeedParser {
public:
  static bool parse(const QByteArray &data, RSSFeed &feed);
};
