# Jni-Study
Android NDK入门，如何建立一个JNI项目和相关配置


## 开始建立第一个android jni project
### 1.环境搭建

开发工具：AndroidStudio
打开AndroidStudio，进入settings,找到Android SDK目录，点击SDK tools，分别找到如图三个配置，勾上，下载。

![image](https://user-images.githubusercontent.com/10016338/133420398-e05acfd8-7522-443d-b8c1-10b69791516f.png)


- 分别介绍一下：
    - cmake
    类似于Android studio中的gradle,是对ndk的一个配置管理
    - LLDB
    c/c++的一个调试工具，安装后可以对c/c++代码进行debug
    - NDK
    ndk的开发工具包

安装完成后，新建一个工程，注意勾选下方的include c++ support,一路next，直到项目构建完成，这样，我们的整体的ndk项目开发环境就配置好了。

### 2.配置文件介绍

项目构建完毕后，我们会发现多了配置和文件，如图：

![image](https://user-images.githubusercontent.com/10016338/133420524-62a36c68-d6df-4c03-a29f-471538f83c02.png)


先介绍一下这几个参数的作用：
点开CMakeLists.txt，如下所示：

```
# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

# 配置so库信息
add_library( # Sets the name of the library.
            # 生成的so库名称，此处生成的so文件名称是libnative-lib.so
             native-lib

             # Sets the library as a shared library.
             # STATIC：静态库，是目标文件的归档文件，在链接其它目标的时候使用
             # SHARED：动态库，会被动态链接，在运行时被加载
             # MODULE：模块库，是不会被链接到其它目标中的插件，但是可能会在运行时使用dlopen-系列的函数动态链接
             SHARED

             # Provides a relative path to your source file(s).
             # 资源文件，可以多个，
             # 资源路径是相对路径，相对于本CMakeLists.txt所在目录
             src/main/cpp/native-lib.cpp )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

# 从系统查找依赖库
find_library( # Sets the name of the path variable.
              # android系统每个类型的库会存放一个特定的位置，而log库存放在log-lib中
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              # android系统在c环境下打log到logcat的库
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

# 配置库的链接（依赖关系）
target_link_libraries( # Specifies the target library.

                       # 目标库
                       native-lib

                       # Links the target library to the log library
                       # included in the NDK.
                       # 依赖于
                       ${log-lib} )

```


看注释就能明白每个方法参数代表什么意思了，如果你觉得里面注释太多，看起来不是很顺眼，可以下载CMake simple highligher插件来提高代码亮度

![image](https://user-images.githubusercontent.com/10016338/133420533-34db6eea-db71-4d39-9294-16f3e033a9fa.png)

![image](https://user-images.githubusercontent.com/10016338/133420542-faeaa3c7-1e39-4c5e-882a-45270e250ebd.png)


关于cmake的配置还有许多，这里只是先简单介绍一个。
我们再来看一下build.gradle中的配置方法的作用,在defultConfig中有个这个参数，
```
 externalNativeBuild {
            cmake {
                cppFlags ""
                 abiFilters 'armeabi','arm64-v8a', 'armeabi-v7a', 'x86', 'x86_64'
            }
        }

```

我们主要关注 abiFilters，它是用来设置编译生成什么类型的so的，我们的cup架构主要有以下几种:

   - armeabi
   - armeabi-v7a
   - arm64-v8a
   - x86
   - x86_64
   - mips
   - mips_64
    其中，armeabi-v7a是目前大部分手机的主流架构，它是基于armeabi的一种升级架构，因此，它们是互相兼容的，但目前各手机厂商的旗舰级，都已经开始采用arm64-v8a的cpu,不过不要慌，它也是兼容armeabi的，所以，如果我们只需要开发普通应用的话，我们只需要关注armeabi和armeabi-v7a就可以了，而像x86等这种类型的架构多用于平板等设备，因此我们只需要了解即可。
    与此相同的还有一个配置：

```
ndk{
            abiFilters "armeabi","armeabi-v7a"
       }

```


它也是用来配置需要什么类型的so的，但它是负责打包到apk里有什么类型的so,而不是编译出什么so,两者还是有区别的。例如我在cmake里设置一个 "armeabi","armeabi-v7a"，在ndk里设置一个armeabi，我make project一下，它会生成两个cpu类型的so,但是我运行打包apk，在apk的lib目录里就只有一个armeabi的so了，所以：
当我们不需要编译so库时，例如引用的第三方库，可以使用ndk来过滤，完全不需要cmake
接下来说一下android下的方法：

```
externalNativeBuild {
        cmake {
            path "CMakeLists.txt"
        }
    }

```

其实很显然，它就是指定我们的CMakeLists文件路径的，当然我们的这个路径是可以更改的。
这里教大家一个小技巧，在cmd中执行：adb shell cat /proc/cpuinfo可以查看当前手机的cpu架构哦，效果图如下：


![image](https://user-images.githubusercontent.com/10016338/133420559-49764c1f-5065-4230-9427-adb27196d70f.png) 





## 分支记录
- [[项目地址]](https://github.com/hutianfeng/Jni-Study/)(https://github.com/hutianfeng/Jni-Study/)

|版本|更新内容|
|:---|:---|
|master|未更新|
|dev_1.0.0|搭建基础的开发环境|



## 参考文件
- [参考文章](https://www.jianshu.com/p/8afbaaaa6931/)(https://www.jianshu.com/p/8afbaaaa6931)

