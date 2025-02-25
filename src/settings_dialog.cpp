//
// Created by ian on 2/23/25.
//
#include "settings_dialog.hpp"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QFontComboBox>
#include <QColorDialog>
#include <QTextEdit>
#include <QPushButton>
#include <QDialogButtonBox>
#include <memory>

struct SettingsDialog::Private {
  RSSFeed &feed;
  QFontComboBox *titleFont;
  QSpinBox *titleSize;
  QFontComboBox *contentFont;
  QSpinBox *contentSize;
  QPushButton *bgColorBtn;
  QPushButton *textColorBtn;
  QPushButton *linkColorBtn;
  QCheckBox *showImages;
  QCheckBox *useCustomCSS;
  QTextEdit *customCSS;
  QString bgColor;
  QString txtColor;
  QString lnkColor;


  explicit Private(RSSFeed& feed) : feed(feed) {}
};



SettingsDialog::SettingsDialog(RSSFeed& feed, QWidget* parent)
    : QDialog(parent), d(std::make_unique<Private>(feed)) {
  setupUI();
}
SettingsDialog::~SettingsDialog() = default;

void SettingsDialog::setupUI() {
  QVBoxLayout *layout;
  layout = new QVBoxLayout(this);
  auto *form = new QFormLayout;

  // Font settings
  d->titleFont = new QFontComboBox;
  d->titleFont->setCurrentFont(QFont(d->feed.formatting.titleFont));
  d->titleSize = new QSpinBox;
  d->titleSize->setValue(d->feed.formatting.titleSize);
  d->contentFont = new QFontComboBox;
  d->contentFont->setCurrentFont(QFont(d->feed.formatting.contentFont));
  d->contentSize = new QSpinBox;
  d->contentSize->setValue(d->feed.formatting.contentSize);

  form->addRow("Title Font:", d->titleFont);
  form->addRow("Title Size:", d->titleSize);
  form->addRow("Content Font:", d->contentFont);
  form->addRow("Content Size:", d->contentSize);

  // Color settings
  d->bgColor = d->feed.formatting.backgroundColor;
  d->txtColor = d->feed.formatting.textColor;
  d->lnkColor = d->feed.formatting.linkColor;

  d->bgColorBtn = new QPushButton("Choose...");
  d->textColorBtn = new QPushButton("Choose...");
  d->linkColorBtn = new QPushButton("Choose...");

  connect(d->bgColorBtn, &QPushButton::clicked, this, [this]() {
    if (const QColor color = QColorDialog::getColor(QColor(d->bgColor), this);
      color.isValid()) {
      d->bgColor = color.name();
    }
  });

  connect(d->textColorBtn, &QPushButton::clicked, this, [this]() {
    if (const QColor color = QColorDialog::getColor(QColor(d->txtColor), this);
      color.isValid()) {
      d->txtColor = color.name();
    }
  });

  connect(d->linkColorBtn, &QPushButton::clicked, this, [this]() {
    if (const QColor color = QColorDialog::getColor(QColor(d->lnkColor), this);
      color.isValid()) {
      d->lnkColor = color.name();
    }
  });

  form->addRow("Background Color:", d->bgColorBtn);
  form->addRow("Text Color:", d->textColorBtn);
  form->addRow("Link Color:", d->linkColorBtn);

  // Other settings
  d->showImages = new QCheckBox;
  d->showImages->setChecked(d->feed.formatting.showImages);
  form->addRow("Show Images:", d->showImages);

  d->useCustomCSS = new QCheckBox;
  d->useCustomCSS->setChecked(d->feed.formatting.useCustomCSS);
  form->addRow("Use Custom CSS:", d->useCustomCSS);

  d->customCSS = new QTextEdit;
  d->customCSS->setPlainText(d->feed.formatting.customCSS);
  form->addRow("Custom CSS:", d->customCSS);

  layout->addLayout(form);

  // Dialog buttons
  auto *buttonBox = new QDialogButtonBox(
      QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  connect(buttonBox, &QDialogButtonBox::accepted, this,
          &SettingsDialog::accept);
  connect(buttonBox, &QDialogButtonBox::rejected, this,
          &SettingsDialog::reject);
  layout->addWidget(buttonBox);

  connect(this, &QDialog::accepted, this, &SettingsDialog::applySettings);
}

void SettingsDialog::applySettings() const {
  d->feed.formatting.titleFont = d->titleFont->currentFont().family();
  d->feed.formatting.titleSize = d->titleSize->value();
  d->feed.formatting.contentFont = d->contentFont->currentFont().family();
  d->feed.formatting.contentSize = d->contentSize->value();
  d->feed.formatting.backgroundColor = d->bgColor;
  d->feed.formatting.textColor = d->txtColor;
  d->feed.formatting.linkColor = d->lnkColor;
  d->feed.formatting.showImages = d->showImages->isChecked();
  d->feed.formatting.useCustomCSS = d->useCustomCSS->isChecked();
  d->feed.formatting.customCSS = d->customCSS->toPlainText();
}
