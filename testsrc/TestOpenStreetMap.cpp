#include <gtest/gtest.h>
#include "OpenStreetMap.h"
#include "XMLReader.h"
#include "StringDataSource.h"
#include <memory>
#include <string>


class OpenStreetMapTest : public ::testing::Test {
protected:
    std::string sampleXML = R"(
<osm>
  <node id="1" lat="40.7128" lon="-74.0060">
    <tag k="highway" v="traffic_signals"/>
  </node>
  <node id="2" lat="34.0522" lon="-118.2437"/>
  <node id="3" lat="51.5074" lon="-0.1278">
    <tag k="amenity" v="restaurant"/>
  </node>
  <way id="100">
    <nd ref="1"/>
    <nd ref="2"/>
    <tag k="highway" v="primary"/>
  </way>
  <way id="101">
    <nd ref="2"/>
    <nd ref="3"/>
    <tag k="highway" v="secondary"/>
  </way>
</osm>
    )";

    std::shared_ptr<CStringDataSource> dataSource;
    std::shared_ptr<CXMLReader> xmlReader;
    std::unique_ptr<COpenStreetMap> osm;

    void SetUp() override {
        dataSource = std::make_shared<CStringDataSource>(sampleXML);
        xmlReader = std::make_shared<CXMLReader>(dataSource);
        osm = std::make_unique<COpenStreetMap>(xmlReader);
    }
};


TEST_F(OpenStreetMapTest, Counts) {
    EXPECT_EQ(osm->NodeCount(), 3u);
    EXPECT_EQ(osm->WayCount(), 2u);
}

TEST_F(OpenStreetMapTest, OrderPreservation) {
    EXPECT_EQ(osm->NodeByIndex(0)->ID(), 1u);
    EXPECT_EQ(osm->NodeByIndex(1)->ID(), 2u);
    EXPECT_EQ(osm->NodeByIndex(2)->ID(), 3u);
    EXPECT_EQ(osm->WayByIndex(0)->ID(), 100u);
    EXPECT_EQ(osm->WayByIndex(1)->ID(), 101u);
}


TEST(OpenStreetMapStandaloneTest, EmptyXML) {
    std::string emptyXML = "<osm></osm>";
    auto dataSource = std::make_shared<CStringDataSource>(emptyXML);
    auto xmlReader = std::make_shared<CXMLReader>(dataSource);
    COpenStreetMap osm(xmlReader);

    EXPECT_EQ(osm.NodeCount(), 0u);
    EXPECT_EQ(osm.WayCount(), 0u);
    EXPECT_EQ(osm.NodeByIndex(0), nullptr);
    EXPECT_EQ(osm.WayByIndex(0), nullptr);
    EXPECT_EQ(osm.NodeByID(1u), nullptr);
    EXPECT_EQ(osm.WayByID(100u), nullptr);
}

TEST(OpenStreetMapStandaloneTest, MultipleAttributes) {
    std::string multiAttrXML = R"(
<osm>
  <node id="1" lat="40.7128" lon="-74.0060">
    <tag k="highway" v="traffic_signals"/>
    <tag k="name" v="Node1"/>
  </node>
  <way id="100">
    <nd ref="1"/>
    <tag k="highway" v="primary"/>
    <tag k="oneway" v="yes"/>
  </way>
</osm>
    )";
    auto dataSource = std::make_shared<CStringDataSource>(multiAttrXML);
    auto xmlReader = std::make_shared<CXMLReader>(dataSource);
    COpenStreetMap osm(xmlReader);

    auto node1 = osm.NodeByIndex(0);
    ASSERT_NE(node1, nullptr);
    EXPECT_EQ(node1->AttributeCount(), 2u);
    EXPECT_TRUE(node1->HasAttribute("highway"));
    EXPECT_EQ(node1->GetAttribute("highway"), "traffic_signals");
    EXPECT_TRUE(node1->HasAttribute("name"));
    EXPECT_EQ(node1->GetAttribute("name"), "Node1");

    auto way100 = osm.WayByIndex(0);
    ASSERT_NE(way100, nullptr);
    EXPECT_EQ(way100->AttributeCount(), 2u);
    EXPECT_TRUE(way100->HasAttribute("highway"));
    EXPECT_EQ(way100->GetAttribute("highway"), "primary");
    EXPECT_TRUE(way100->HasAttribute("oneway"));
    EXPECT_EQ(way100->GetAttribute("oneway"), "yes");
}