#include "UIBase.h"

#define PARENT 0


void UIBase::SetFocus( UIBase * keyFocus )
{
    if( !m_Focus )
    {
        if( m_vControl[ PARENT ] )
            m_vControl[ PARENT ]->SetFocus( keyFocus );
        else
        {
            if( GetFocus( ) )
                GetFocus( )->m_Focus = false;
            m_pUIFocus = keyFocus;
            m_Focus = true;
        }
    }
}

bool UIBase::PostMessage( UINT msg, WPARAM wParam, LPARAM lParam, void * Data )
{
    switch( msg )
    {
    case WM_LBUTTONDOWN:
        if( CursorIntersect( LOWORD( lParam ), HIWORD( lParam ) ) )
        {
            UIBase* ChildControl = PostToAll( msg, wParam, lParam, Data );
            if(!ChildControl )
                OnMouseDown( msg, LOWORD( lParam ), HIWORD( lParam ) );
            return true;
        }
        else
            return false;
        break;
    case WM_MOUSEMOVE:
        if( CursorIntersect( LOWORD( lParam ), HIWORD( lParam ) ) )
        {
            m_Focus = true;
            UIBase* ChildControl = PostToAll( msg, wParam, lParam, Data );
            if( !ChildControl )
                OnMouseMove( LOWORD( lParam ), HIWORD( lParam ) );
            return true;
        }
        else
            return false;
        break;
    case WM_KEYUP:
    case WM_KEYDOWN:
        if( GetFocus( ) )
        {
            if( msg == WM_KEYUP )
                GetFocus( )->OnKeyUp( wParam, lParam );
            if( msg == WM_KEYDOWN )
                GetFocus( )->OnKeyDown( wParam, lParam );
        }
        break;
    default:
        break;
    }
    return false;
}

bool UIBase::CursorIntersect( FLOAT x, FLOAT y )
{
    D3DXVECTOR2 l_AbsolutPosTR, l_AbsolutPosBL, l_CursorPos;
    //variable for the top right corner 
    l_AbsolutPosTR.x = 0;
    l_AbsolutPosTR.y = 0;

    //variable for the bottom left corner
    l_AbsolutPosBL.x = 0;
    l_AbsolutPosBL.y = 0;
    
    //putting the cursor position into a vector
    l_CursorPos.x = x;
    l_CursorPos.y = y;
    
    GetAbsolutePosition( &l_AbsolutPosTR );
    GetAbsolutePosition( &l_AbsolutPosBL );
    l_AbsolutPosBL.x += GetWidth( );
    l_AbsolutPosBL.y += GetHeight( );

    if( (l_CursorPos <= l_AbsolutPosTR) || (l_CursorPos >= l_AbsolutPosBL) )
        return true;

    return false;
}

UIBase * UIBase::PostToAll( UINT msg, WPARAM wParam, LPARAM lParam, void * Data )
{
    for(UINT i=1;i <= m_vControl.size( );i++ )
    {
        if( m_vControl[i]->PostMessage( msg, wParam, lParam, Data ) )
            return m_vControl[ i ];
    }
    return NULL;
}

UIBase::UIBase( UIBase * parent, int vecPos )
{
	m_ChildCount = 0;
    m_vControl.clear( );
    m_vControl.push_back( parent );
    m_thisVecPos = vecPos;
}

void UIBase::GetAbsolutePosition( D3DXVECTOR2 * Position )
{
    D3DXVec2Add( Position, Position, &m_Position );
}

UIBase::~UIBase( )
{
}
