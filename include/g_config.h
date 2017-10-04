#ifndef PURECPP_H
#define PURECPP_H

#define UNUSED(x)                   (void)(x)
#define START_FUNC()                std::cout<<"--- Start "<<__PRETTY_FUNCTION__<<std::endl
#define SHOW_NAME_AND_RESULT(n)     std::cout<<#n" return "<< (n)<<std::endl

// 检查两种类型是否基本一致
template <typename T, typename U>
struct is_decay_same :
    std::is_same<typename std::decay<T>::type, U>::type
{};

#endif // PURECPP_H
