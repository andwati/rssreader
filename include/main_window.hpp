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
class QTimer;
class QComboBox;

class RSSReader : public QMainWindow {
  Q_OBJECT

public:
  explicit RSSReader(QWidget *parent = nullptr);

  ~RSSReader();

protected:
  void closeEvent(QCloseEvent *event) override;

private:
  void setupUI();
  void setupConnections();
  void setupRefreshTimer();
  void handleNetworkReply(QNetworkReply *reply);
  void loadSettings();
  void saveSettings();
  void refreshFeeds();

private slots:
  void addNewFeed();
  void addNewCategory();
  void loadFeedContent(int index);

  void displayArticle(int index);
  void categoryChanged(const QString &category);
  void markAsRead();
  void markAllAsRead();

private:
  struct Private;
  std::unique_ptr<Private> d;
};
