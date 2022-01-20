#include "Avatar.hpp"

USING_NS_CC;

using namespace std;
using namespace base;

Avatar::Avatar()
{
    m_animManager.attach(this);
}

Avatar::~Avatar()
{
}

void Avatar::loadAnimationList(const std::vector<std::string>& fileList)
{
    m_animManager.loadList(fileList);
}
