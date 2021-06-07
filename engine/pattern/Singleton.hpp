//
//  Singleton.hpp
//  MiniRPG
//
//  Created by nuttachai on 13/9/20.
//

#ifndef Singleton_h
#define Singleton_h

namespace pattern
{
    template <typename _Class>
    class Singleton
    {
    public:
        static _Class& getInstance()
        {
            static _Class* _instance = nullptr;
            
            if (!_instance) {
                _instance = new _Class();
            }
            
            return *_instance;
        }
        
    protected:
        Singleton() {}
        Singleton(const Singleton&) {}
        virtual ~Singleton() {}
    };
}

#endif /* Singleton_h */
