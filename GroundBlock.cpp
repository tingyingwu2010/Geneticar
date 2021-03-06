/*
 *  GroundBlock.cpp
 *  FirstBox2D
 *
 *  Created by PRoTeW on 10/05/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "GroundBlock.h"
#include "GLWorldViewer.h"

GroundBlock :: GroundBlock( int ID, QGLWidget * QGL, b2Vec2 *P, b2Vec2 *D )
: Object( ID, dynamic_cast <GLWorldViewer*> ( QGL )->getWorld(), P, D ){
	
	angle = 0;
	body_def = new b2BodyDef;
	body_def->type = b2_staticBody;

    image = new QImage( ":/images/base.png" );
	
	canvas = QGL;

	body_def->position.Set( position->x, position->y );

	body = world->CreateBody( body_def );
	shape = new b2PolygonShape;
	shape->SetAsBox( dimension->x, dimension->y, body->GetWorldCenter(), angle );

	fixture_definition = new b2FixtureDef;
	fixture_definition->shape = shape;
	fixture_definition->density = 1.0f;
	fixture_definition->friction = 1.0f;
	fixture_definition->filter.groupIndex = -6;
	FixData * fix_data			= new FixData;
	fix_data->object_type		= GROUND;
	fixture_definition->userData = fix_data;
	fixture = body->CreateFixture( fixture_definition );
	createDisplayList();
}
GroundBlock :: GroundBlock( int ID, QGLWidget * QGL, b2Vec2 *P, b2Vec2 *D, float32 A  )
: Object( ID, dynamic_cast <GLWorldViewer*> ( QGL )->getWorld(), P, D ){
	
	angle = A;

    image = nullptr;
	
	canvas = QGL;
	
	body_def = new b2BodyDef;
	body_def->position.Set( position->x, position->y );
	body = world->CreateBody( body_def );
	shape = new b2PolygonShape;
	shape->SetAsBox( dimension->x, dimension->y, body->GetPosition(), angle );
	//SetAsBox( float32 hx, float32 hy, const b2Vec2& center, float32 angle );
	fixture_definition = new b2FixtureDef;
	fixture_definition->shape = shape;
	fixture_definition->density = 1.0f;
	fixture_definition->friction = 0.75f;
	body->CreateFixture( fixture_definition );
	createDisplayList();
}
void GroundBlock :: createDisplayList( void )
{
	//PEGA UM NUMERO DE LISTA DISPONÍVEL
	display_list = glGenLists( 1 );
	//INVOCA A LISTA
	glNewList( display_list, GL_COMPILE );
	//PREENCHE COM OS VÉRTICES
	fillDisplayList();
	//FIM DA CRIAÇÃO 
	glEndList();
}
void GroundBlock :: fillDisplayList( void )
{
	glColor4f( 0.0f, 0.0f, 0.0f, 1.0f );
	glBegin( GL_LINE_LOOP );
	for( int i = 0; i < shape->GetVertexCount(); i++ )
        glVertex2d( static_cast<double> ( shape->GetVertex(i).x ), static_cast<double> ( shape->GetVertex(i).y ) );
	glEnd();
    if( !image->isNull() )
	{
		glEnable( GL_TEXTURE_2D );
        texture = canvas->bindTexture( *image );
	}
	glColor4f( 0.5f, 0.5f, 0.5f, 1.0f );
    //
    glBegin( GL_QUADS );
	for( int i = 0; i < shape->GetVertexCount(); i++ )
	{
		switch ( i ) {
			case 0:
				glTexCoord2f( 0, 0 );
				break;
			case 1:
				glTexCoord2f( 1, 0 );
				break;
			case 2:
				glTexCoord2f( 1, 1 );
				break;
			case 3:
				glTexCoord2f( 0, 1 );
				break;
		}
        glVertex2d( static_cast<double>( shape->GetVertex( i ).x ), static_cast<double>( shape->GetVertex( i ).y ) );
	}
	glEnd();
	glDisable( GL_TEXTURE_2D );
}
void GroundBlock :: draw( void )
{
	glPushMatrix();
	
    glTranslated( static_cast<double>( body->GetPosition().x ), static_cast<double>( body->GetPosition().y ), 0.0 );
	glRotatef( ( body->GetAngle() / b2_pi ) * 180, 0, 0, 1 );
	
	glCallList( display_list );
	
	glPopMatrix(); 
}
GroundBlock :: ~GroundBlock( void ){}
