#include <gtest/gtest.h>
#include "CSVBusSystem.h"
#include "DSVReader.h"
#include "BusSystem.h"
#include "StringDataSource.h"
#include <string>
#include <vector>

class TestBusSystem : public ::testing::Test {
protected:
    void SetUp() override {
        std::string stopData = "stopd_id,node_id\n22043,2849810514\n22358,2849805223\n22105,3954294407\n";
        std::string routeData = "route,stop_id\nA,22258\nA,22169\nA,22000\n";

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
};

TEST_F(TestBusSystem, StopCount) {
    EXPECT_EQ(busSystem->StopCount(), 3u);
}

TEST_F(TestBusSystem, RouteCount) {
    EXPECT_EQ(busSystem->RouteCount(), 1u);
}

TEST_F(TestBusSystem, StopByIndex) {
    auto stop0 = busSystem->StopByIndex(0);
    ASSERT_NE(stop0, nullptr);
    EXPECT_EQ(stop0->ID(), 22043u);
    EXPECT_EQ(stop0->NodeID(), 2849810514u);

    auto stop1 = busSystem->StopByIndex(1);
    ASSERT_NE(stop1, nullptr);
    EXPECT_EQ(stop1->ID(), 22358u);

    auto stop2 = busSystem->StopByIndex(2);
    ASSERT_NE(stop2, nullptr);
    EXPECT_EQ(stop2->ID(), 22105u);

    auto stopInvalid = busSystem->StopByIndex(3);
    EXPECT_EQ(stopInvalid, nullptr);

    auto stopLargeIndex = busSystem->StopByIndex(1000);
    EXPECT_EQ(stopLargeIndex, nullptr);
}

TEST_F(TestBusSystem, StopByID) {
    auto stop22043 = busSystem->StopByID(22043u);
    ASSERT_NE(stop22043, nullptr);
    EXPECT_EQ(stop22043->ID(), 22043u);
    EXPECT_EQ(stop22043->NodeID(), 2849810514u);

    auto stop22105 = busSystem->StopByID(22105u);
    ASSERT_NE(stop22105, nullptr);
    EXPECT_EQ(stop22105->ID(), 22105u);

    auto stopInvalid = busSystem->StopByID(99999u);
    EXPECT_EQ(stopInvalid, nullptr);
}

TEST_F(TestBusSystem, RouteByIndex) {
    auto route0 = busSystem->RouteByIndex(0);
    ASSERT_NE(route0, nullptr);
    EXPECT_EQ(route0->Name(), "A");
    EXPECT_EQ(route0->StopCount(), 3u);

    auto route1 = busSystem->RouteByIndex(1);
    EXPECT_EQ(route1, nullptr);

    auto routeLargeIndex = busSystem->RouteByIndex(1000);
    EXPECT_EQ(routeLargeIndex, nullptr);
}

TEST_F(TestBusSystem, RouteByName) {
    auto routeA = busSystem->RouteByName("A");
    ASSERT_NE(routeA, nullptr);
    EXPECT_EQ(routeA->Name(), "A");
    EXPECT_EQ(routeA->StopCount(), 3u);

    auto routeB = busSystem->RouteByName("B");
    EXPECT_EQ(routeB, nullptr);
}

TEST_F(TestBusSystem, RouteStopIDs) {
    auto routeA = busSystem->RouteByName("A");
    ASSERT_NE(routeA, nullptr);

    EXPECT_EQ(routeA->GetStopID(0), 22258u);
    EXPECT_EQ(routeA->GetStopID(1), 22169u);
    EXPECT_EQ(routeA->GetStopID(2), 22000u);
    EXPECT_EQ(routeA->GetStopID(3), CBusSystem::InvalidStopID);
}