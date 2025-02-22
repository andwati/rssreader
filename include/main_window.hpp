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
class QMenu;

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
  void setupMenus();
  void setupRefreshTimer();
  void handleNetworkReply(QNetworkReply *reply);
  void loadSettings();
  void saveSettings();
  void refreshFeeds();
  void refreshFeed(RSSFeed &feed);
  QString formatContent(const FeedItem &item, const RSSFeed &feed) const;

private slots:
  void addNewFeed();
  void deleteFeed();
  void moveFeedUo();
  void moveFeedDown();
  void editFeedSettings();
  void loadFeedContent(int index);
  void displayArticle(int index);
  void refreshCurentFeed();
  void refreshAllFeeds();
  void addNewCategory();
  void showContextMenu(const QPoint &pos);
  void categoryChanged(const QString &category);
  void markAsRead();
  void markAllAsRead();

private:
  struct Private;
  std::unique_ptr<Private> d;
};
