/*
	This file is part of Overmix.

	Overmix is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Overmix is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Overmix.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "FullscreenViewer.hpp"

#include "viewer/imageCache.h"


FullscreenViewer::FullscreenViewer( QSettings& settings, imageCache* cache, QWidget* parent ) : imageViewer( settings ){
	change_image( cache, true );
	setStyleSheet( "* {background: black;}" );
	if( parent ){
		move( parent->pos() );
	}
	showFullScreen();
	setAttribute( Qt::WA_DeleteOnClose, true );
	
	setFocusPolicy( Qt::ClickFocus );
	setFocus( Qt::PopupFocusReason );
	connect( this, SIGNAL(double_clicked()), this, SLOT(close()) );
}

FullscreenViewer::FullscreenViewer( QSettings& settings, QImage img, QWidget* parent )
	:	FullscreenViewer( settings, new imageCache( img ), parent ) { }

void FullscreenViewer::keyPressEvent( QKeyEvent* event ){
	switch( event->key() ){
		case Qt::Key_F11:
		case Qt::Key_Escape: close(); break;
		default: event->ignore();
	}
}

