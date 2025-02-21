//
// Created by ian on 2/22/25.
//

#pragma once
#include "feed.hpp"
#include <QMainWindow>
#include <memory>
#include <vector>

class QListWidget;
class QTextBrowser;
class QPushButton;
class QNetworkAccessManager;
class QNetworkReply;

class RSSReader : public QMainWindow {
  Q_OBJECT

public:
  explicit RSSReader(QWidget *parent = nullptr);

  ~RSSReader();

private:
  void setupUI();

  void setupConnections();

  void handleNetworkReply(QNetworkReply *reply);

private slots:
  void addNewFeed();

  void loadFeedContent(int index);

  void displayArticle(int index);

private:
  struct Private;
  std::unique_ptr<Private> d;
};
