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
  QComboBox *categoryCombo{nullptr};
  QPushButton *addCategoryButton{nullptr};
  QPushButton *markReadButton{nullptr};
  QPushButton *markAllReadButton{nullptr};
  QTimer *refreshTimer{nullptr};
  std::vector<FeedCategory> categories;
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

  // Add category management
  auto *categoryLayout = new QHBoxLayout();
  d->categoryCombo = new QComboBox();
  d->categoryCombo->addItem("All");
  d->addCategoryButton = new QPushButton("Add Category");
  categoryLayout->addWidget(d->categoryCombo);
  categoryLayout->addWidget(d->addCategoryButton);
  leftPanel->insertLayout(0, categoryLayout);

  // Add read status management
  auto *readLayout = new QHBoxLayout();
  d->markReadButton = new QPushButton("Mark Read");
  d->markAllReadButton = new QPushButton("Mark All Read");
  readLayout->addWidget(d->markReadButton);
  readLayout->addWidget(d->markAllReadButton);
  rightPanel->addLayout(readLayout);

  loadSettings();
  setupRefreshTimer();

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

  connect(d->addCategoryButton, &QPushButton::clicked, this,
          &RSSReader::addNewCategory);
  connect(d->categoryCombo, &QComboBox::currentTextChanged, this,
          &RSSReader::categoryChanged);
  connect(d->markReadButton, &QPushButton::clicked, this,
          &RSSReader::markAsRead);
  connect(d->markAllReadButton, &QPushButton::clicked, this,
          &RSSReader::markAllAsRead);
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


void RSSReader::setupRefreshTimer() {
    d->refreshTimer = new QTimer(this);
    connect(d->refreshTimer, &QTimer::timeout, this, &RSSReader::refreshFeeds);
    d->refreshTimer->start(5 * 60 * 1000); // 5 minutes
}

void RSSReader::loadSettings() {
    FeedStorage::instance().loadCategories(d->categories);
    for (const auto& category : d->categories) {
        d->categoryCombo->addItem(category.name);
    }

    std::vector<std::unique_ptr<RSSFeed>> feeds;
    if (FeedStorage::instance().loadFeeds(feeds)) {
        d->feeds = std::move(feeds);
        for (const auto& feed : d->feeds) {
            d->feedList->addItem(feed->feedTitle);
        }
    }
}

void RSSReader::saveSettings() {
    FeedStorage::instance().saveFeeds(d->feeds);
    FeedStorage::instance().saveCategories(d->categories);
}

void RSSReader::closeEvent(QCloseEvent* event) {
    saveSettings();
    QMainWindow::closeEvent(event);
}

void RSSReader::addNewCategory() {
    bool ok;
    QString name = QInputDialog::getText(this, "Add Category",
                                       "Enter category name:", QLineEdit::Normal,
                                       "", &ok);
    if (ok && !name.isEmpty()) {
        FeedCategory category;
        category.name = name;
        d->categories.push_back(category);
        d->categoryCombo->addItem(name);
    }
}

void RSSReader::categoryChanged(const QString& category) {
    d->feedList->clear();
    for (const auto& feed : d->feeds) {
        if (category == "All" || feed->category == category) {
            d->feedList->addItem(feed->feedTitle);
        }
    }
}

void RSSReader::markAsRead() {
    int feedIndex = d->feedList->currentRow();
    int articleIndex = d->articleList->currentRow();
    if (feedIndex >= 0 && articleIndex >= 0) {
        d->feeds[feedIndex]->items[articleIndex].isRead = true;
        QListWidgetItem* item = d->articleList->item(articleIndex);
        QFont font = item->font();
        font.setItalic(true);
        item->setFont(font);
    }
}

void RSSReader::markAllAsRead() {
    int feedIndex = d->feedList->currentRow();
    if (feedIndex >= 0) {
        for (auto& item : d->feeds[feedIndex]->items) {
            item.isRead = true;
        }
        for (int i = 0; i < d->articleList->count(); ++i) {
            QListWidgetItem* item = d->articleList->item(i);
            QFont font = item->font();
            font.setItalic(true);
            item->setFont(font);
        }
    }
}