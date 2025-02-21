//
// Created by ian on 2/22/25.
//
#include "main_window.hpp"
#include "feed_parser.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QTextBrowser>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QInputDialog>
#include <QMessageBox>

struct RSSReader::Private {
  QWidget *centralWidget{nullptr};
  QListWidget *feedList{nullptr};
  QListWidget *articleList{nullptr};
  QTextBrowser *contentView{nullptr};
  QPushButton *addFeedButton{nullptr};
  QNetworkAccessManager *networkManager{nullptr};
  std::vector<std::unique_ptr<RSSFeed>> feeds;
};

RSSReader::RSSReader(QWidget *parent)
  : QMainWindow(parent)
    , d(std::make_unique<Private>()) {
  setupUI();
  setupConnections();
}

RSSReader::~RSSReader() = default;

void RSSReader::setupUI() {
  d->centralWidget = new QWidget(this);
  setCentralWidget(d->centralWidget);

  auto *mainLayout = new QHBoxLayout(d->centralWidget);
  auto *leftPanel = new QVBoxLayout();

  d->feedList = new QListWidget();
  d->addFeedButton = new QPushButton("Add Feed");
  leftPanel->addWidget(d->feedList);
  leftPanel->addWidget(d->addFeedButton);

  d->articleList = new QListWidget();
  d->contentView = new QTextBrowser();
  d->contentView->setOpenExternalLinks(true);

  auto *rightPanel = new QVBoxLayout();
  rightPanel->addWidget(d->articleList);
  rightPanel->addWidget(d->contentView);

  mainLayout->addLayout(leftPanel, 1);
  mainLayout->addLayout(rightPanel, 2);

  d->networkManager = new QNetworkAccessManager(this);

  resize(800, 600);
  setWindowTitle("Modern RSS Reader");
}

void RSSReader::setupConnections() {
  connect(d->addFeedButton, &QPushButton::clicked, this,
          &RSSReader::addNewFeed);
  connect(d->feedList, &QListWidget::currentRowChanged, this,
          &RSSReader::loadFeedContent);
  connect(d->articleList, &QListWidget::currentRowChanged, this,
          &RSSReader::displayArticle);
}

void RSSReader::addNewFeed() {
  bool ok;
  QString url = QInputDialog::getText(this, "Add Feed",
                                      "Enter RSS feed URL:", QLineEdit::Normal,
                                      "https://", &ok);
  if (ok && !url.isEmpty()) {
    auto feed = std::make_unique<RSSFeed>();
    feed->feedUrl = url;
    d->feeds.push_back(std::move(feed));

    QNetworkRequest request((QUrl(url)));
    QNetworkReply *reply = d->networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
      handleNetworkReply(reply);
    });
  }
}

void RSSReader::handleNetworkReply(QNetworkReply *reply) {
  if (reply->error() == QNetworkReply::NoError) {
    auto &currentFeed = d->feeds.back();
    if (FeedParser::parse(reply->readAll(), *currentFeed)) {
      d->feedList->addItem(currentFeed->feedTitle);
    } else {
      QMessageBox::warning(this, "Error", "Failed to parse feed");
    }
  } else {
    QMessageBox::warning(this, "Error",
                         "Failed to fetch feed: " + reply->errorString());
  }
  reply->deleteLater();
}

void RSSReader::loadFeedContent(int index) {
  if (index >= 0 && index < static_cast<int>(d->feeds.size())) {
    d->articleList->clear();
    const auto &feed = d->feeds[index];
    for (const auto &item : feed->items) {
      d->articleList->addItem(item.title);
    }
  }
}

void RSSReader::displayArticle(int index) {
  int feedIndex = d->feedList->currentRow();
  if (feedIndex >= 0 && index >= 0) {
    const auto &feed = d->feeds[feedIndex];
    if (index < static_cast<int>(feed->items.size())) {
      const auto &item = feed->items[index];
      QString content = QString("<h2>%1</h2>").arg(item.title);
      content += QString("<p><i>Published: %1</i></p>").arg(item.pubDate);
      content += QString("<p>%1</p>").arg(item.description);
      content += QString("<p><a href='%1'>Read more...</a></p>").arg(item.link);
      d->contentView->setHtml(content);
    }
  }
}
