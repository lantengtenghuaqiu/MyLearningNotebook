1.“初始化”: 无法从“const _Elem”转换为“const vec3 &”:
    实例:
        模板特化判断:
            namespace types
            {
                template<typename T ,typename U>
                struct is_same 
                {
                    static constexpr bool value = false;
                };
        

                template<typename T>
                struct is_same<T,T>
                {
                    static constexpr bool value = true;
                };
            }

        class vec3;
        static std::ofstream LOGER_FILE_STREAM("loger.log",std::ios::app);

        template<typename T>
        inline static void DOLOGE(const std::initializer_list<T> args)
        {   
            if(LOGER_FILE_STREAM.is_open())
            {
                xyl::types::is_same<T,vec3> typeCheck;
                if(typeCheck.value)     --出错地方
                {
                    LOGER_FILE_STREAM<<"["<<__DATE__<<" : "<<__LINE__<<" : "<<__TIME__<<"] : "; 
                    #ifdef BINARYREAD
                        for(auto it = args.begin();it != args.end();it++)
                        {   
                            LOGER_FILE_STREAM<<*it;
                        }
                    #else
                        for(const vec3& it : args)
                        {
                            LOGER_FILE_STREAM<<it.x()<<"\t\t"<<it.y()<<"\t\t"<<it.z()<<"\n";         
                        }
                        
                    #endif
                }
                else
                {
                    for(const T& it : args)
                    {
                        LOGER_FILE_STREAM<<"["<<__DATE__<<" : "<<__LINE__<<" : "<<__TIME__<<"] : "<<it<<"\t"<<std::endl;
                    }
                }
            }
        }
    在使用模板特化判断两个模板是否相同的时候,进行if判断,即使传入的args类型与模板比对不同,即为false,依然会走if(true)的判断,并进行报错,显示为,两个类型不同且无法转换.
    解决:
        因为if()是在运行时判断,但是在编译时无论条件判断为true还是false,都会进行遍历,所以在编译的时候这两个类型无法互相转换,则报错;
        因此需要在条件判断中添加修饰符if constexpr(){},如此在编译期间就会进行判断,如果是false则跳过该分支.
        此外,对于if constexpr(<param>)中的<param>可以在编译期间进行初始化并判断是因为,特化模板中的参数也是constexpr