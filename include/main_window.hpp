//
// Created by ian on 2/22/25.
//

#pragma once
#include "feed.hpp"
#include <QMainWindow>
#include <memory>


class QListWidget;
class QTextBrowser;
class QPushButton;
class QNetworkAccessManager;
class QNetworkReply;
class QComboBox;


class RSSReader final : public QMainWindow {
  Q_OBJECT

public:
  explicit RSSReader(QWidget *parent = nullptr);

  ~RSSReader() override;

protected:
  void closeEvent(QCloseEvent *event) override;

private:
  void setupUI();
  void setupConnections();
  void setupMenus();
  void setupRefreshTimer();
  void handleNetworkReply(QNetworkReply *reply);
  void loadSettings();
  void saveSettings() const;
  void refreshFeeds();
  void refreshFeed(const RSSFeed &feed);
  static QString formatContent(const FeedItem &item, const RSSFeed &feed) ;

private slots:
  void addNewFeed();
  void deleteFeed();
  void moveFeedUp();
  void moveFeedDown();
  void editFeedSettings();
  void loadFeedContent(int index);
  void displayArticle(int index);
  void refreshCurrentFeed();
  void refreshAllFeeds();
  void addNewCategory();
  void showContextMenu(const QPoint &pos);
  void categoryChanged(const QString &category) const;
  void markAsRead() const;
  void markAllAsRead() const;

private:
  struct Private;
  std::unique_ptr<Private> d;
};
