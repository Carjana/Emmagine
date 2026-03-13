#pragma once
#include <map>
#include <typeindex>


class IService
{

};

inline std::map<std::type_index, IService*> Services;

template<typename TService>
TService* GetService()
{
	std::map<std::type_index, IService *>::iterator it = Services.find(typeid(TService));
	if (it != Services.end())
	{
		return static_cast<TService *>(it->second);
	}
	return nullptr;
}

template<typename TService>
void RegisterService(TService *service)
{
	Services.try_emplace(typeid(TService), service);
}