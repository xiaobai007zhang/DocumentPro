// 头文件
// hhoking 2019.6.11
//
#pragma once
#include "ys_dlloader.h"
#include "ys_lock.h"
#include "ys_util.h"
#include "ys_ibase.h"
#include <string>
#include <unordered_map>
using namespace std;

#ifdef PF_WINDOWS
#if (defined(USE_DEBUG) && defined(_DEBUG))
#define _modname    "%sd.dll"
#else
#define _modname    "%s.dll"
#endif
#else
#if (defined(USE_DEBUG) && defined(_DEBUG))
#define _modname    "lib%sd.so"
#else
#define _modname    "lib%s.so"
#endif
#endif

/* @hhoking@ 动态加载核心库框架
    注意核心函数并不是无状态的，所以核心模块不能即用即加载，
    必须加载后缓存起来
*/
namespace ys
{
    typedef IBase*(*CREATE_COMPONENT)(uint cid, IBase* outer);

    class dllcache
    {
    private:
        unordered_map<string, dlloader*>    _mods;
        typedef unordered_map<string, dlloader*>::const_iterator citerator;
        typedef unordered_map<string, dlloader*>::iterator iterator;
        lock _lock;

    public:
        dllcache() {}
        ~dllcache() { Free(); }

    public:
        // 将指定标识的模块缓存（如果已缓存直接返回true）
        bool Cache(pcchar flag)
        {
            autolock lock(_lock);

            citerator iter = _mods.find(flag);
            if (iter == _mods.end())
            {
                StringA modname;
                format_string(modname, _modname, flag);
                dlloader* mod = new dlloader();
                if (!mod->Load(modname))
                {
                    delete mod;
                    printf("%s not found, %s!\n", (pcchar)modname, mod->Error());
                    return false;
                }
                _mods[flag] = mod;
            }
            return true;
        }

        // 将指定标识的模块解除缓存
        void Uncache(pcchar flag)
        {
            autolock lock(_lock);

            iterator iter = _mods.find(flag);
            if (iter != _mods.end())
            {
                delete iter->second;
                _mods.erase(iter);
            }
        }

        void Free()
        {
            autolock lock(_lock);

            for (iterator iter = _mods.begin(); iter != _mods.end(); iter++)
            {
                delete iter->second;
                iter->second = NULL;
            }
            _mods.clear();
        }

        dlloader* Dll(pcchar flag)
        {
            return _mods[flag];
        }

        CREATE_COMPONENT CreateComponent(pcchar flag)
        {
            StringA funcname;
            format_string(funcname, "%sCreateComponent", flag);
            return (CREATE_COMPONENT)_mods[flag]->GetProc(funcname);
        }
    };
}
