#include <gtest/gtest.h>
#include "CSVBusSystem.h"
#include "DSVReader.h"
#include "StringDataSource.h"
#include <string>
#include <vector>

class TestBusSystem: public ::testing::Test {
    protected:
        void SetUp() override{
            std::string stopData = "stopd_id,node_id\n22043,2849810514\n22358,2849805223\n;
            22105,3954294407"
            std::string routeData "route,stop_id\nA,22258\nA,22169\nA,22000";
            
            stopSrc = std::make_shared<CStringDataSource>(stopData);
            routeSrc = std::make_shared<CStringDataSource>(routeData);
            stopReader = std::make_shared<CDSVReader>(stopSrc, ',');
            routeReader = std::make_shared<CDSVReader>(routeSrc, ',');
            busSystem = std::make_unique<CCSVBusSystem>(stopReader, routeReader);

        }
    
    std::shared_ptr<CStringDataSource> stopSrc;
    std::shared_ptr<CStringDataSource> routeSrc;
    std::shared_ptr<CDSVReader> stopReader;
    std::shared_ptr<CDSVReader> routeReader;
    std::unique_ptr<CCSVBusSystem> busSystem;
}

TEST_F(TestBusSystem, StopCount){
    EXPECT_EQ(busSystem->StopCount(), 3);
}

TEST_F(TestBusSystem, RouteCount){
    EXPECT_EQ(busSystem->RouteCount(), 3);
}

TEST_F(TestBusSystem, StopByIndex){
    auto stop = TestBusSystem->StopByIndex(0);
    ASSERT_NE(stop, nullptr) << "Stop at index 0 should exist";
}