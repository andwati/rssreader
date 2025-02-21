//
// Created by ian on 2/22/25.
//
#include "feed_parser.hpp"
#include <QXmlStreamReader>

bool FeedParser::parse(const QByteArray& data, RSSFeed& feed) {
    QXmlStreamReader xml(data);

    while (!xml.atEnd()) {
        if (xml.readNextStartElement()) {
            if (xml.name() == "channel") {
                while (xml.readNextStartElement()) {
                    if (xml.name() == "title") {
                        feed.feedTitle = xml.readElementText();
                    } else if (xml.name() == "item") {
                        FeedItem item;
                        while (xml.readNextStartElement()) {
                            if (xml.name() == "title")
                                item.title = xml.readElementText();
                            else if (xml.name() == "link")
                                item.link = xml.readElementText();
                            else if (xml.name() == "description")
                                item.description = xml.readElementText();
                            else if (xml.name() == "pubDate")
                                item.pubDate = xml.readElementText();
                            else
                                xml.skipCurrentElement();
                        }
                        feed.items.push_back(std::move(item));
                    } else {
                        xml.skipCurrentElement();
                    }
                }
            }
        }
    }

    return !xml.hasError();
}