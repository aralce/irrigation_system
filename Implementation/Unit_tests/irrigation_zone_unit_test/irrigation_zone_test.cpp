#include <gtest/gtest.h>
#include "irrigation_zone.h"

/*************************
 *  Irrigation Zone Test (Irrigation zone is not threat safe.)
 * ***********************
 *  The main requirements are:
 * 1- Irrigation zone must be constructed with a reference to an electrovalve and a pump.
 * 2- If the electrovalve and pump have both sensors, the irrigation zone is sensing system health.
 * 3- If the electrovalve doesn't have a sensor, the irrigation zone is not sensing system health.
 * 4- If the pump doesn't have a sensor, the irrigation zone is not sensing system health.
 * 5- Irrigation zone inits as no irrigating.
 * 6- Irrigation zone can be put to irrigate.
 * 7- If irrigation zone is put to irrigate and pump is off, then pump is ON.
 * 8- Irrigation zone can be stoped from irrigating.
 * 9- If two irrigation zones are irrigating. Each irrigation zone knows how many zones are being irrigating.
 * 10- If two irrigation zones are irrigating and 1 of them stop irrigating, then the count of irrigating zones is decreased by 1.
 * 11- Stop irrigating a zone twice, only drecrease the count of irrigating zones by 1.
 * 12- Start irrigating a zone twice, only increase the count of irrigating zones by 1.
 * 13- If irrigation zone is turn off, pump is ON and there isn't any other irrigation zone active, so pump is off.
 * 
 * ****************************************************************************
 * Author: Ariel Cerfoglia
 * Email: ariel.cerfoglia@gmail.com
 * ****************************************************************************
*/ 

//Setting fixture to have SetUp and TearDown functions
class IrrigationZoneTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
      
    }  
    //void TearDown() override {}
MockActuator electrovalve;
MockActuator pump;
};


/*STARTING TESTS*/

//1- Irrigation zone must be constructed with a reference to an electrovalve and a pump.
TEST_F(IrrigationZoneTest, constructor) {
  Irrigation_zone irr(electrovalve, pump);  
  EXPECT_EQ(&Debug::get_electrovalve(irr), &electrovalve);
  EXPECT_EQ(&Debug::get_pump(irr), &pump);
};

//2- If the electrovalve and pump have both sensors, the irrigation zone is sensing system health.
TEST_F(IrrigationZoneTest, irrigation_sensing_health) {
  Irrigation_zone irr(electrovalve, pump);
  using ::testing::Return;
  EXPECT_CALL(electrovalve, has_sensor)
    .Times(1)
    .WillOnce(Return(true));
  EXPECT_CALL(pump, has_sensor)
    .Times(1)
    .WillOnce(Return(true));
  EXPECT_EQ(true, irr.is_sensing_health());
};

//3- If the electrovalve doesn't have a sensor, the irrigation zone is not sensing system health.
TEST_F(IrrigationZoneTest, irrigation_electrovalve_not_sensing) {
  Irrigation_zone irr(electrovalve, pump);
  using ::testing::Return;
  EXPECT_CALL(electrovalve, has_sensor)
    .Times(1)
    .WillOnce(Return(false));
  EXPECT_EQ(false, irr.is_sensing_health());
};

//4- If the pump doesn't have a sensor, the irrigation zone is not sensing system health.
TEST_F(IrrigationZoneTest, irrigation_pump_not_sensing) {
  Irrigation_zone irr(electrovalve, pump);
  using ::testing::Return;
  EXPECT_CALL(electrovalve, has_sensor)
    .Times(1)
    .WillOnce(Return(true));
  EXPECT_CALL(pump, has_sensor)
    .Times(1)
    .WillOnce(Return(false));
  EXPECT_EQ(false, irr.is_sensing_health());
};

//5- Irrigation zone inits as no irrigating.
TEST_F(IrrigationZoneTest, irrigation_init)
{
  Irrigation_zone irr(electrovalve, pump);
  using ::testing::Return;
  EXPECT_CALL(electrovalve, is_ON)
    .WillOnce(Return(false));
  EXPECT_EQ(false, irr.is_irrigating());  
}

//6- Irrigation zone can be put to irrigate.
TEST_F(IrrigationZoneTest, irrigation_set_ON)
{
  Irrigation_zone irr(electrovalve, pump);
  EXPECT_CALL(electrovalve, set(true));
  irr.irrigate(true);
  using ::testing::Return;
  EXPECT_CALL(electrovalve, is_ON)
    .WillOnce(Return(true));
  EXPECT_EQ(true, irr.is_irrigating());
}

//7- If irrigation zone is put to irrigate and pump is off, then pump is ON.
TEST_F(IrrigationZoneTest, irrigation_set_ON_PUMP)
{
  Irrigation_zone irr(electrovalve, pump);
  using ::testing::Return;
  EXPECT_CALL(pump, is_ON)
    .WillOnce(Return(false));
  EXPECT_CALL(pump, set(true));
  irr.irrigate(true);
}

//8- Irrigation zone can be stoped from irrigating.
TEST_F(IrrigationZoneTest, irrigation_set_OFF)
{
  Irrigation_zone irr(electrovalve, pump);
  irr.irrigate(true);
  using ::testing::Return;
  EXPECT_CALL(electrovalve, is_ON)
    .WillOnce(Return(true));
  EXPECT_CALL(electrovalve, set(false));
  irr.irrigate(false);
}

//9- If two irrigation zones are irrigating. Each irrigation zone knows how many zones are being irrigating.
TEST_F(IrrigationZoneTest, irrigation_2_zones_set_ON)
{
  Irrigation_zone irr(electrovalve, pump);
  MockActuator electrovalve2;
  Irrigation_zone irr2(electrovalve2, pump);
  EXPECT_EQ( 0, Debug::get_nzones_irrigating());
  EXPECT_CALL(electrovalve, set(true));
  irr.irrigate(true);
  EXPECT_EQ( 1, Debug::get_nzones_irrigating());
  EXPECT_CALL(electrovalve2, set(true));
  irr2.irrigate(true);
  EXPECT_EQ( 2, Debug::get_nzones_irrigating());
}

//10- If two irrigation zones are irrigating and 1 of them stop irrigating, then the count of irrigating zones is decreased by 1.
TEST_F(IrrigationZoneTest, irrigation_2_zones_set_ON_1_OFF)
{
  Irrigation_zone irr(electrovalve, pump);
  MockActuator electrovalve2;
  Irrigation_zone irr2(electrovalve2, pump);
  irr.irrigate(true);
  irr2.irrigate(true);
  EXPECT_EQ( 2, Debug::get_nzones_irrigating());
  using ::testing::Return;
  EXPECT_CALL(electrovalve2, is_ON)
    .WillOnce(Return(true));
  irr2.irrigate(false);
  EXPECT_EQ( 1, Debug::get_nzones_irrigating());
}

//11- Stop irrigating a zone twice, only drecrease the count of irrigating zones by 1.
TEST_F(IrrigationZoneTest, irrigation_zones_set_ON_then_OFF)
{
  Irrigation_zone irr(electrovalve, pump);
  irr.irrigate(true);
  EXPECT_EQ( 1, Debug::get_nzones_irrigating());
  using ::testing::Return;
  EXPECT_CALL(electrovalve, is_ON)
    .WillOnce(Return(true));
  irr.irrigate(false);
  EXPECT_EQ( 0, Debug::get_nzones_irrigating());
  EXPECT_CALL(electrovalve, is_ON)
    .WillOnce(Return(false));
  irr.irrigate(false);
  EXPECT_EQ( 0, Debug::get_nzones_irrigating());
}

//12- Start irrigating a zone twice, only increase the count of irrigating zones by 1.
TEST_F(IrrigationZoneTest, irrigation_zones_set_ON_twice)
{
  Irrigation_zone irr(electrovalve, pump);
  irr.irrigate(true);
  EXPECT_EQ( 1, Debug::get_nzones_irrigating());
  using ::testing::Return;
  EXPECT_CALL( electrovalve, is_ON)
    .WillOnce(Return(true));
  irr.irrigate(true);
  EXPECT_EQ( 1, Debug::get_nzones_irrigating());
}

//13- If irrigation zone is turn off, pump is ON and there isn't any other irrigation zone active, so pump is off.
TEST_F(IrrigationZoneTest, irrigation_zones_set_OFF_pump_OFF)
{
  Irrigation_zone irr(electrovalve, pump);
  irr.irrigate(true);
  EXPECT_EQ( 1, Debug::get_nzones_irrigating());
  using ::testing::Return;
  EXPECT_CALL( pump, is_ON)
    .WillOnce(Return(true));
  EXPECT_CALL( pump, set(false));
  irr.irrigate(false);
}
