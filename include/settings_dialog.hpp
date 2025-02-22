//
// Created by ian on 2/23/25.
//
#pragma once
#include "feed.hpp"
#include <QDialog>
#include <memory>

class SettingsDialog : public QDialog {
  Q_OBJECT
public:
  explicit SettingsDialog(RSSFeed& feed, QWidget* parent = nullptr);
  ~SettingsDialog();

private:
  void setupUI();
  void applySettings();

  struct Private;
  std::unique_ptr<Private> d;
};