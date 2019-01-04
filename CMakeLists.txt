cmake_minimum_required(VERSION 3.0)

project(mlgraphs-bandits-network)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost COMPONENTS graph)

include_directories(${Boost_INCLUDE_DIRS})

set(BANDIT_FILES
        bandit/Bandit.h bandit/Bandit.h bandit/IArm.h bandit/BanditNetwork.h bandit/ArmBernoulli.h)

set(NETWORK_FILES
        network/Network.h network/Edge.h network/NetworkFactory.h)

set(POLICY_FILES
        policy/IPolicy.h
        policy/UCBNetworkPolicy.cpp
        policy/UCBNetworkPolicy.h
        policy/FollowYourLeaderPolicy.h
        policy/FollowYourLeaderPolicy.cpp
        monte-carlo/MonteCarlo.h
        monte-carlo/MonteCarlo.cpp
        policy/FollowBestInformedPolicy.h
        policy/FollowBestInformedPolicy.cpp)
set(UTILS_FILES
        util/Statistics.cpp
        util/Statistics.h
        util/GraphViz.h
        util/ThreadPool.h
        util/ThreadSafeQueue.h)


set(SOURCE_FILES ${BANDIT_FILES} ${NETWORK_FILES} ${POLICY_FILES} ${UTILS_FILES})

add_executable(mlgraphs-bandits-network main.cpp ${SOURCE_FILES})
target_link_libraries(mlgraphs-bandits-network pthread)
target_link_libraries(mlgraphs-bandits-network ${Boost_LIBRARIES})
