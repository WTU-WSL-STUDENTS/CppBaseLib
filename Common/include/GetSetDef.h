/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-10-03 21:01:51
 * @LastEditors: like
 * @LastEditTime: 2021-10-03 21:02:58
 */
#ifndef COMPIERDEF_H
#define COMPIERDEF_H

#define DECLARE_GET(Type, param)\
        protected:\
        Type param;\
        public:\
        Type Get##param##(){return param;}\

#define DECLARE_GETSET(Type, param)\
        protected:\
        Type param;\
        public:\
        Type Get##param##(){return param;}\
        void Set##param##(Type val){param = val;}

#define DECLARE_PTR(type, param)\
        public:\
            type param;\
            void Init##param## (int length)\
            {\
                if(NULL == param)\
                {\
                    param = (type)malloc(length);\
                }\
            }\
            void Dispose##param## ()\
            {\
                if(NULL != param)\
                {\
                    free(param);\
                    param = NULL;\
                }\
            }

#define DECLARE_TEMPLATE_PTR(param)\
        public:\
        T* param;\
        void Init##param## (int length)\
        {\
            if(NULL == param)\
            {\
                param = (T*)malloc(length);\
            }\
        }\
        void Dispose##param## ()\
        {\
            if(NULL != param)\
            {\
                free(param);\
                param = NULL;\
            }\
        }

        
#endif