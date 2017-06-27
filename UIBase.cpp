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
	case WM_LBUTTONUP:
	case WM_LBUTTONDOWN:
        if( CursorIntersect( LOWORD( lParam ), HIWORD( lParam ) ) )
        {
            UIBase* ChildControl = PostToAll( msg, wParam, lParam, Data );
			if (!ChildControl)
			{
				OnMouseUp(msg, LOWORD(lParam), HIWORD(lParam));
				OnMouseDown(msg, LOWORD(lParam), HIWORD(lParam));
				m_MUClient.BufferIn("Intersect");
				m_MUClient.Send();
			}
            return true;
        }
        else
            return false;
        break;

    case MM_JOY1MOVE:
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
		{
			OnMouseMove(LOWORD(lParam), HIWORD(lParam));
			m_MUClient.BufferIn("Mouse Move");
			m_MUClient.Send();
			return false;
		}
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
	case WM_PAINT:
		OnRender();
		if (m_vControl.size() > 1)
			PostToAllReverse(msg, wParam, lParam, Data);
		break;
    default:
        break;
    }
    return false;
}

bool UIBase::CursorIntersect( FLOAT x, FLOAT y )
{
    D3DXVECTOR2 l_AbsolutPosTL, l_AbsolutPosBR;
    //variable for the top right corner 
	l_AbsolutPosTL.x = 0;
	l_AbsolutPosTL.y = 0;

    //variable for the bottom left corner
	l_AbsolutPosBR.x = 0;
	l_AbsolutPosBR.y = 0;
    
    
    GetAbsolutePosition( &l_AbsolutPosTL);
    GetAbsolutePosition( &l_AbsolutPosBR);
	l_AbsolutPosBR.x += GetWidth( );
	l_AbsolutPosBR.y += GetHeight( );

    if( ((x >= l_AbsolutPosTL.x) && (x <= l_AbsolutPosBR.x)) && ((y >= l_AbsolutPosTL.y) && (y <= l_AbsolutPosBR.y)))
        return true;

    return false;
}

UIBase * UIBase::PostToAll( UINT msg, WPARAM wParam, LPARAM lParam, void * Data )
{
    for(UINT i=1;i < m_vControl.size( );i++ )
    {
        if( m_vControl[i]->PostMessage( msg, wParam, lParam, Data ) )
            return m_vControl[ i ];
    }
    return NULL;
}

UIBase * UIBase::PostToAllReverse(UINT msg, WPARAM wParam, LPARAM lParam, void * Data)
{
	for (UINT i = m_vControl.size()-1; i >0; i--)//goes backwards through the vector excluding the first element which is a pointer to the parent
	{
		m_vControl[i]->PostToAllReverse(msg, wParam, lParam, Data);
	}
	PostMessage(msg, wParam, lParam, Data);
	return nullptr;
}

UIBase::UIBase( UIBase * parent, int vecPos)
{
	m_MUClient.Initialize();
	m_ChildCount = 0;
    m_vControl.clear( );
    m_vControl.push_back( parent );
    m_thisVecPos = vecPos;
}

UIBase * UIBase::AddChildControl(UIBase * child)
{
	m_vControl.push_back(child);//add the child to this controls children
	child->SetSprite(this->GetSprite());// propagate the sprite to the child
	return nullptr;
}

void UIBase::SetTexture(Texture * tex)
{
	m_Texture = tex;
	SetHeight(m_Texture->GetHeight());
	SetWidth(m_Texture->GetWidth());
}

void UIBase::GetAbsolutePosition( D3DXVECTOR2 * Position )
{
    D3DXVec2Add( Position, Position, &m_Position );
}

UIBase::~UIBase( )
{
}
