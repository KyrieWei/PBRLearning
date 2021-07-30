#include "MeshMgr.h"

template<> MeshMgr::ptr Singleton<MeshMgr>::_instance = nullptr;

MeshMgr::ptr MeshMgr::getSingleton()
{
	if (_instance == nullptr)
		_instance = std::make_shared<MeshMgr>();
	return _instance;
}