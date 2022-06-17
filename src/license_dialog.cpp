
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2022 Igor Mironchik

	Permission is hereby granted, free of charge, to any person
	obtaining a copy of this software and associated documentation
	files (the "Software"), to deal in the Software without
	restriction, including without limitation the rights to use,
	copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following
	conditions:

	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	OTHER DEALINGS IN THE SOFTWARE.
*/

// Widgets include.
#include "license_dialog.hpp"
#include "ui_license_dialog.h"

// Qt include.
#include <QScrollArea>
#include <QLabel>
#include <QFrame>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPalette>
#include <QScrollBar>


//
// Anchor
//

class Anchor
	:	public QWidget
{
public:
	Anchor( QWidget * parent, const QString & name,
		QLabel * label, QScrollArea * scroll )
		:	QWidget( parent )
		,	m_label( label )
		,	m_scroll( scroll )
		,	m_pressed( false )
	{
		setToolTip( name );
		setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
		setMinimumSize( 20, 10 );
	}

protected:
	void mousePressEvent( QMouseEvent * e ) override
	{
		if( e->button() == Qt::LeftButton )
			m_pressed = true;

		e->accept();
	}

	void mouseReleaseEvent( QMouseEvent * e ) override
	{
		if( m_pressed && e->button() == Qt::LeftButton )
		{
			m_pressed = false;

			m_scroll->verticalScrollBar()->setSliderPosition( m_label->y() );
		}

		e->accept();
	}

	void paintEvent( QPaintEvent * ) override
	{
		QPainter p( this );
		p.setRenderHint( QPainter::Antialiasing );

		p.setPen( QPen( palette().color( QPalette::Highlight ), 2.0 ) );
		p.setBrush( palette().color( QPalette::Button ) );

		QPainterPath pt;
		pt.moveTo( 1, 1 );
		pt.lineTo( width() - 1, 1 );
		pt.lineTo( width() - height() / 2, height() / 2 );
		pt.lineTo( width() - 1, height() - 1 );
		pt.lineTo( 1, height() - 1 );
		pt.lineTo( 1, 1 );

		p.drawPath( pt );
	}

private:
	QLabel * m_label;
	QScrollArea * m_scroll;
	bool m_pressed;
}; // class Anchor


//
// LicenseDialogPrivate
//

class LicenseDialogPrivate {
public:
	LicenseDialogPrivate()
		:	m_w( nullptr )
		,	m_l( nullptr )
		,	m_aw( nullptr )
		,	m_al( nullptr )
	{
	}

	void init( LicenseDialog * q )
	{
		m_w = new QWidget( q );
		m_l = new QVBoxLayout( m_w );
		m_aw = new QWidget( q );
		m_al = new QVBoxLayout( m_aw );
		m_al->setContentsMargins( 2, 2, 2, 2 );
		m_al->setSpacing( 2 );
		m_al->addItem( new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding ) );

		m_ui.setupUi( q );

		m_ui.m_scrollArea->setWidget( m_w );
		m_ui.m_anchors->setWidget( m_aw );
		m_ui.m_anchors->setMinimumWidth( 20 + 4 +
			m_ui.m_anchors->verticalScrollBar()->sizeHint().width() );
	}

	//! Ui.
	Ui::LicenseDialog m_ui;
	//! Widget.
	QWidget * m_w;
	//! Layout.
	QVBoxLayout * m_l;
	//! Anchors widget.
	QWidget * m_aw;
	//! Anchors layout.
	QVBoxLayout * m_al;
}; // class LicenseDialogPrivate



//
// LicenseDialog
//

LicenseDialog::LicenseDialog( QWidget * parent )
	:	QDialog( parent )
	,	d( new LicenseDialogPrivate )
{
	d->init( this );
}

LicenseDialog::~LicenseDialog()
{
}

void
LicenseDialog::addLicense( const QString & title, const QString & license )
{
	const bool first = ( d->m_l->count() == 0 );

	if( !first )
	{
		QFrame * line = new QFrame( d->m_w );
		line->setFrameStyle( QFrame::HLine | QFrame::Plain );
		d->m_l->addWidget( line );
	}

	QLabel * label = new QLabel( d->m_w );
	label->setText( license );
	label->setWordWrap( true );
	label->setTextInteractionFlags( Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard |
		Qt::LinksAccessibleByMouse | Qt::LinksAccessibleByKeyboard );
	label->setOpenExternalLinks( true );

	d->m_l->addWidget( label );

	Anchor * a = new Anchor( d->m_aw, title, label, d->m_ui.m_scrollArea );
	d->m_al->insertWidget( d->m_al->count() - 1, a );
}
