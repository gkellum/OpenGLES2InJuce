#include "TreeNode.h"

#include "FreeTypeBitmapChar.h"

Allocator<TreeNode> TreeNode::m_allocator;

TreeNode::TreeNode()
{
    m_pLeaf1 = NULL;
    m_pLeaf2 = NULL;
}

TreeNode::TreeNode(int x, int y, int width, int height)
{
    m_x = x;
    m_y = y;
    m_width = width;
    m_height = height;
    m_pLeaf1 = NULL;
    m_pLeaf2 = NULL;
}

void TreeNode::Set(int x, int y, int width, int height)
{
    m_x = x;
    m_y = y;
    m_width = width;
    m_height = height;
    m_pLeaf1 = NULL;
    m_pLeaf2 = NULL;
}

TreeNode::~TreeNode()
{
}

void TreeNode::CreateBranches(FreeTypeBitmapCharPtr pFTBitmapChar)
{
    int dx = m_width - pFTBitmapChar->GetWidth();
    int dy = m_height - pFTBitmapChar->GetHeight();
    // we split to give one very small leaf and one very big one
    // because it allows more efficent use of space
    // if you don't do this, the bottom right corner never gets used
    if (dx < dy)    
    {
        //  split so the top is cut in half and the rest is one big rect below
        m_pLeaf1 = m_allocator.New();
        m_pLeaf1->Set(m_x + pFTBitmapChar->GetWidth(), m_y, 
                m_width - pFTBitmapChar->GetWidth(), 
                pFTBitmapChar->GetHeight());
        m_pLeaf2 = m_allocator.New();
        m_pLeaf2->Set(m_x, m_y + pFTBitmapChar->GetHeight(), 
                m_width, m_height - pFTBitmapChar->GetHeight());
    }
    else
    {
        //  m_pLeaf1 = left (cut in half)
        m_pLeaf1 = m_allocator.New();
        m_pLeaf1->Set(m_x, m_y + pFTBitmapChar->GetHeight(), 
                pFTBitmapChar->GetWidth(), m_height - pFTBitmapChar->GetHeight());
        // m_pLeaf2 = right (not cut)
        m_pLeaf2 = m_allocator.New();
        m_pLeaf2->Set(m_x + pFTBitmapChar->GetWidth(), m_y, 
                m_width - pFTBitmapChar->GetWidth(), m_height);
    }
}

bool TreeNode::Add(FreeTypeBitmapCharPtr pFTBitmapChar)
{
    if (pFTBitmapChar->IsEmpty()) return true;
    if (IsEmpty())
    {   
        if (Fits(pFTBitmapChar))
        {
            CreateBranches(pFTBitmapChar);
            pFTBitmapChar->SetXY(m_x, m_y);
            return true;
        }
        return false;
    }
    if (m_pLeaf1->Add(pFTBitmapChar)) 
    {
        return true;
    }
    if (m_pLeaf2->Add(pFTBitmapChar)) 
    {
        return true;
    }
    return false;       
}

bool TreeNode::Fits(FreeTypeBitmapCharPtr pFTBitmapChar)
{
    return pFTBitmapChar->GetWidth() <= m_width && pFTBitmapChar->GetHeight() <= m_height;
}

