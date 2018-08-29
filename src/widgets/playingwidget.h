/*
 * Strawberry Music Player
 * This file was part of Clementine.
 * Copyright 2010, David Sansome <me@davidsansome.com>
 * Copyright 2013, Jonas Kvinge <jonas@strawbs.net>
 *
 * Strawberry is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Strawberry is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Strawberry.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef PLAYINGWIDGET_H
#define PLAYINGWIDGET_H

#include "config.h"

#include <memory>
#include <stdbool.h>

#include <QtGlobal>
#include <QObject>
#include <QWidget>
#include <QString>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QSize>
#include <QSignalMapper>
#include <QTextDocument>
#include <QTimeLine>
#include <QAction>
#include <QActionGroup>
#include <QMenu>
#include <QMovie>
#include <QtEvents>

#include "core/song.h"
#include "covermanager/albumcoverloaderoptions.h"

using std::unique_ptr;

class QContextMenuEvent;
class QDragEnterEvent;
class QDropEvent;
class QMouseEvent;
class QPaintEvent;
class QResizeEvent;

class AlbumCoverChoiceController;
class Application;

class PlayingWidget : public QWidget {
  Q_OBJECT

 public:
  PlayingWidget(QWidget *parent = nullptr);
  ~PlayingWidget();

  void SetApplication(Application *app);
  void SetEnabled();
  void SetDisabled();
  void set_ideal_height(int height);
  QSize sizeHint() const;

 signals:
  void ShowAboveStatusBarChanged(bool above);

 public slots:
  void Playing();
  void Stopped();
  void Error();
  void SongChanged(const Song &song);

 protected:
  void paintEvent(QPaintEvent *e);
  void resizeEvent(QResizeEvent*);
  void contextMenuEvent(QContextMenuEvent *e);
  void mouseReleaseEvent(QMouseEvent*);
  void dragEnterEvent(QDragEnterEvent *e);
  void dropEvent(QDropEvent *e);

 private slots:

  void SetMode(int mode);
  void FitCoverWidth(bool fit);

  void LoadCoverFromFile();
  void SaveCoverToFile();
  void LoadCoverFromURL();
  void SearchForCover();
  void UnsetCover();
  void ShowCover();
  void SearchCoverAutomatically();
  void AutomaticCoverSearchDone();

  void AlbumArtLoaded(const Song &song, const QString &uri, const QImage &image);
  void SetHeight(int height);  
  void FadePreviousTrack(qreal value);

 private:

  enum Mode {
    SmallSongDetails = 0,
    LargeSongDetails = 1,
  };

  static const char *kSettingsGroup;
  static const int kPadding;
  static const int kGradientHead;
  static const int kGradientTail;
  static const int kMaxCoverSize;
  static const int kBottomOffset;
  static const int kTopBorder;

  Application *app_;
  AlbumCoverChoiceController *album_cover_choice_controller_;
  Mode mode_;
  QMenu *menu_;
  QAction *fit_cover_width_action_;
  bool enabled_;
  bool visible_;
  bool active_;
  int small_ideal_height_;
  AlbumCoverLoaderOptions cover_loader_options_;
  int total_height_;
  bool fit_width_;
  QTimeLine *timeline_show_hide_;
  QTimeLine *timeline_fade_;
  QTextDocument *details_;
  qreal pixmap_previous_track_opacity_;
  bool downloading_covers_;

  Song song_;
  QImage image_original_;
  QPixmap pixmap_cover_;
  QPixmap pixmap_previous_track_;
  std::unique_ptr<QMovie> spinner_animation_;

  void SetVisible(bool visible);
  void CreateModeAction(Mode mode, const QString &text, QActionGroup *group, QSignalMapper *mapper);
  void UpdateDetailsText();
  void UpdateHeight();
  void SetImage(const QImage &image);
  void DrawContents(QPainter *p);
  void ScaleCover();
  bool GetCoverAutomatically();

};

#endif  // PLAYINGWIDGET_H
