cmake_minimum_required(VERSION 3.13)
project(DonkeyKong_master)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(DonkeyKong_master
        DonkeyKong.cpp
        Entity.cpp
        Entity.h
        EntityManager.cpp
        EntityManager.h
        Game.cpp
        Game.h
        pch.cpp
        pch.h
        StringHelpers.cpp
        StringHelpers.h)
