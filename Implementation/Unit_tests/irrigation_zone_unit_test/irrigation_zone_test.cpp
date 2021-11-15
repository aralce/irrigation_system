#include <gtest/gtest.h>
#include "irrigation_zone.h"

/*************************
 *  Irrigation Zone Test (Irrigation zone is not threat safe.)
 * ***********************
 *  The main requirements are:
 * //Constructor
 * 1- Irrigation zone must be constructed with two pointers to two actuators.
 * 
 * //IsSensing zone health
 * 2.1- If the electrovalve and pump have both sensors, the irrigation zone is sensing system health.
 * 2.2- If the electrovalve doesn't have a sensor, the irrigation zone is not sensing system health.
 * 2.3- If the pump doesn't have a sensor, the irrigation zone is not sensing system health.
 * 2.4- If the pump or the electrovalve are not initialized, the irrigation zone is not sensing system health. The operation has no effect
 * 
 * //Irrigate zone
 * 3.1- Irrigation zone inits as no irrigating.
 * 3.2- Irrigation a zone with null pump and null electrovalve, are considering not irrigating. The function has no effect.
 * 3.3- Irrigation zone can be put to irrigate.
 * 3.4- Irrigate an irrigation zone with null electrovalve and null pump has no effect.
 * 3.5- If irrigation zone is put to irrigate and pump is off, then pump is ON.
 * 3.6- Irrigation zone can be stoped from irrigating.
 * 
 * //Manage irrigation
 * 4.1- If two irrigation zones are irrigating. Each irrigation zone knows how many zones are being irrigating.
 * 4.2- If two irrigation zones are irrigating and 1 of them stop irrigating, then the count of irrigating zones is decreased by 1.
 * 4.3- Stop irrigating a zone twice, only drecrease the count of irrigating zones by 1.
 * 4.4- Start irrigating a zone twice, only increase the count of irrigating zones by 1.
 * 4.5- If irrigation zone is turn off, pump is ON and there isn't any other irrigation zone active, so pump is off.
 * 
 * //Sense zone health
 * 5.1- When the pump sensor and electrovalve sensor are both equal to uint32(MAX value), then irrigation zone is healthy.
 * 5.2- When the irrigation zone is not sensing health, then irrigation zone is considered healthy
 * 5.3- When the pump sensor reading less electrovalve sensor reading is lesser or equal than HEALTH_OFFSET, then irrigation zone is healthy.
 * 5.4- When the pump sensor reading less electrovalve sensor reading is greater than HEALTH_OFFSET, then irrigation zone is NOT healthy.
 * 5.5- When the pump or the electrovalve are not initialized, then irrigation zone is NOT healthy. The function has no effect.
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
      irr = new Irrigation_zone(&electrovalve, &pump);
    }  
    //void TearDown() override {}
MockActuator electrovalve;
MockActuator pump;
Irrigation_zone *irr = nullptr;
};

/*STARTING TESTS*/
////////
//Constructor
//1- Irrigation zone must be constructed with a reference to an electrovalve and a pump.
TEST_F(IrrigationZoneTest, constructor){
  EXPECT_EQ(Debug::get_electrovalve(irr), &electrovalve);
  EXPECT_EQ(Debug::get_pump(irr), &pump);
};

////////////
//IsSensing zone health
//2.1- If the electrovalve and pump have both sensors, the irrigation zone is sensing system health.
TEST_F(IrrigationZoneTest, irrigation_sensing_health){
  using ::testing::Return;
  EXPECT_CALL(electrovalve, has_sensor)
    .Times(1)
    .WillOnce(Return(true));
  EXPECT_CALL(pump, has_sensor)
    .Times(1)
    .WillOnce(Return(true));
  EXPECT_EQ(true, irr->is_sensing_health());
};

//2.2- If the electrovalve doesn't have a sensor, the irrigation zone is not sensing system health.
TEST_F(IrrigationZoneTest, irrigation_electrovalve_not_sensing){
  using ::testing::Return;
  EXPECT_CALL(electrovalve, has_sensor)
    .Times(1)
    .WillOnce(Return(false));
  EXPECT_EQ(false, irr->is_sensing_health());
};

//2.3- If the pump doesn't have a sensor, the irrigation zone is not sensing system health.
TEST_F(IrrigationZoneTest, irrigation_pump_not_sensing){
  using ::testing::Return;
  EXPECT_CALL(electrovalve, has_sensor)
    .Times(1)
    .WillOnce(Return(true));
  EXPECT_CALL(pump, has_sensor)
    .Times(1)
    .WillOnce(Return(false));
  EXPECT_EQ(false, irr->is_sensing_health());
};

//2.4- If the pump or the electrovalve are not initialized, the irrigation zone is not sensing system health. The operation has no effect
TEST_F(IrrigationZoneTest, irrigation_sensing_with_null_pump_null_electrovalve){
  MockActuator *electrovalve_null = nullptr;
  MockActuator *pump_null = nullptr;
  Irrigation_zone irrigation(electrovalve_null, pump_null);
  EXPECT_EQ(false, irrigation.is_sensing_health());
};


/////////
//Irrigate zone
//3.1- Irrigation zone inits as no irrigating.
TEST_F(IrrigationZoneTest, irrigation_init){
  using ::testing::Return;
  EXPECT_CALL(electrovalve, is_ON)
    .WillOnce(Return(false));
  EXPECT_FALSE(irr->is_irrigating());
  EXPECT_CALL(electrovalve, is_ON)
    .WillOnce(Return(false));
  EXPECT_CALL(pump, set(true))
    .Times(0);
  EXPECT_CALL(pump, set(false))
    .Times(1);
  irr->irrigate(false);
}

//3.2- Irrigation a zone with null pump and null electrovalve, are considering not irrigating. The function has no effect.
TEST_F(IrrigationZoneTest, irrigation_init_with_null_pump_null_electrovalve){
  MockActuator *electrovalve_null = nullptr;
  MockActuator *pump_null = nullptr;
  Irrigation_zone irrigation(electrovalve_null, pump_null);
  EXPECT_EQ(false, irrigation.is_irrigating());  
}

//3.3- Irrigation zone can be put to irrigate.
TEST_F(IrrigationZoneTest, irrigation_set_ON){
  EXPECT_CALL(electrovalve, set(true));
  irr->irrigate(true);
  using ::testing::Return;
  EXPECT_CALL(electrovalve, is_ON)
    .WillOnce(Return(true));
  EXPECT_EQ(true, irr->is_irrigating());
}

//3.4- Irrigate an irrigation zone with null electrovalve and null pump has no effect.
TEST_F(IrrigationZoneTest, irrigation_set_ON_with_null_electrovalve_and_null_pump){
  MockActuator *electrovalve_null = nullptr;
  MockActuator *pump_null = nullptr;
  Irrigation_zone irrigation(electrovalve_null, pump_null);
  irrigation.irrigate(true);
  EXPECT_EQ(false, irrigation.is_irrigating());
}

//3.5- If irrigation zone is put to irrigate and pump is off, then pump is ON.
TEST_F(IrrigationZoneTest, irrigation_set_ON_PUMP){
  using ::testing::Return;
  EXPECT_CALL(pump, is_ON)
    .WillOnce(Return(false));
  EXPECT_CALL(pump, set(true));
  irr->irrigate(true);
}

//3.6- Irrigation zone can be stoped from irrigating.
TEST_F(IrrigationZoneTest, irrigation_set_OFF){
  irr->irrigate(true);
  using ::testing::Return;
  EXPECT_CALL(electrovalve, is_ON)
    .WillOnce(Return(true));
  EXPECT_CALL(electrovalve, set(false));
  irr->irrigate(false);
}

//////////
//Manage irrigation
//4.1- If two irrigation zones are irrigating. Each irrigation zone knows how many zones are being irrigating.
TEST_F(IrrigationZoneTest, irrigation_2_zones_set_ON){
  MockActuator electrovalve2;
  Irrigation_zone irr2(&electrovalve2, &pump);
  EXPECT_EQ( 0, Debug::get_nzones_irrigating());
  EXPECT_CALL(electrovalve, set(true));
  irr->irrigate(true);
  EXPECT_EQ( 1, Debug::get_nzones_irrigating());
  EXPECT_CALL(electrovalve2, set(true));
  irr2.irrigate(true);
  EXPECT_EQ( 2, Debug::get_nzones_irrigating());
}

//4.2- If two irrigation zones are irrigating and 1 of them stop irrigating, then the count of irrigating zones is decreased by 1.
TEST_F(IrrigationZoneTest, irrigation_2_zones_set_ON_1_OFF){
  MockActuator electrovalve2;
  Irrigation_zone irr2(&electrovalve2, &pump);
  irr->irrigate(true);
  irr2.irrigate(true);
  EXPECT_EQ( 2, Debug::get_nzones_irrigating());
  using ::testing::Return;
  EXPECT_CALL(electrovalve2, is_ON)
    .WillOnce(Return(true));
  irr2.irrigate(false);
  EXPECT_EQ( 1, Debug::get_nzones_irrigating());
}

//4.3- Stop irrigating a zone twice, only drecrease the count of irrigating zones by 1.
TEST_F(IrrigationZoneTest, irrigation_zones_set_ON_then_OFF){
  irr->irrigate(true);
  EXPECT_EQ( 1, Debug::get_nzones_irrigating());
  using ::testing::Return;
  EXPECT_CALL(electrovalve, is_ON)
    .WillOnce(Return(true));
  irr->irrigate(false);
  EXPECT_EQ( 0, Debug::get_nzones_irrigating());
  EXPECT_CALL(electrovalve, is_ON)
    .WillOnce(Return(false));
  irr->irrigate(false);
  EXPECT_EQ( 0, Debug::get_nzones_irrigating());
}

//4.4- Start irrigating a zone twice, only increase the count of irrigating zones by 1.
TEST_F(IrrigationZoneTest, irrigation_zones_set_ON_twice){
  irr->irrigate(true);
  EXPECT_EQ( 1, Debug::get_nzones_irrigating());
  using ::testing::Return;
  EXPECT_CALL( electrovalve, is_ON)
    .WillOnce(Return(true));
  irr->irrigate(true);
  EXPECT_EQ( 1, Debug::get_nzones_irrigating());
}

//4.5- If irrigation zone is turn off, pump is ON and there isn't any other irrigation zone active, so pump is off.
TEST_F(IrrigationZoneTest, irrigation_zones_set_OFF_pump_OFF){
  EXPECT_EQ( 0, Debug::get_nzones_irrigating());
  irr->irrigate(true);
  EXPECT_EQ( 1, Debug::get_nzones_irrigating());
  using ::testing::Return;
  EXPECT_CALL( electrovalve, is_ON)
    .WillRepeatedly(Return(true));
  EXPECT_CALL( pump, set(false));
  irr->irrigate(false);
  EXPECT_EQ( 0, Debug::get_nzones_irrigating());
}

////////////
//Sense zone health
//5.1- When the pump sensor and electrovalve sensor are both equal to uint32(MAX value), then irrigation zone is healthy.
TEST_F(IrrigationZoneTest, irrigation_zones_healthy_with_switch_sensor){
  using ::testing::Return;
  EXPECT_CALL(pump, has_sensor)
    .WillOnce(Return(true));
  EXPECT_CALL(electrovalve, has_sensor)
    .WillOnce(Return(true));
  EXPECT_CALL(pump, read_sensor())
    .WillOnce(Return(std::numeric_limits<uint32_t>::max()));
  EXPECT_CALL(electrovalve, read_sensor())
    .WillOnce(Return(std::numeric_limits<uint32_t>::max()));  
  EXPECT_EQ(true, irr->is_healthy());
}

//5.2- When the irrigation zone is not sensing health, then irrigation zone is considered healthy
TEST_F(IrrigationZoneTest, irrigation_zones_healthy_not_sensing){
  EXPECT_EQ(false, irr->is_sensing_health());
  EXPECT_EQ(true, irr->is_healthy());
}

//5.3- When the pump sensor reading less electrovalve sensor reading is lesser or equal than HEALTH_OFFSET, then irrigation zone is healthy.
TEST_F(IrrigationZoneTest, irrigation_zones_healthy_with_analog_sensor){
  using ::testing::Return;
  EXPECT_CALL(pump, has_sensor)
    .WillOnce(Return(true));
  EXPECT_CALL(electrovalve, has_sensor)
    .WillOnce(Return(true));
  EXPECT_CALL(pump, read_sensor)
    .WillOnce(Return(HEALTHY_OFFSET));
  EXPECT_CALL(electrovalve, read_sensor)
    .WillOnce(Return(0));
  EXPECT_EQ( true, irr->is_healthy());
}

//5.4- When the pump sensor reading less electrovalve sensor reading is greater than HEALTH_OFFSET, then irrigation zone is NOT healthy.
TEST_F(IrrigationZoneTest, irrigation_zones_not_healthy_with_analog_sensor){
  constexpr uint32_t OFFSET_ADDED = 2000;
  using ::testing::Return;
  EXPECT_CALL(pump, has_sensor)
    .WillOnce(Return(true));
  EXPECT_CALL(electrovalve, has_sensor)
    .WillOnce(Return(true));
  EXPECT_CALL(pump, read_sensor)
    .WillOnce(Return(HEALTHY_OFFSET + OFFSET_ADDED));
  EXPECT_CALL(electrovalve, read_sensor)
    .WillOnce(Return(OFFSET_ADDED - 1));
  EXPECT_EQ( false, irr->is_healthy());
}

//5.5- When the pump or the electrovalve are not initialized, then irrigation zone is NOT healthy. The function has no effect.
TEST_F(IrrigationZoneTest, irrigation_zones_not_healthy_not_initialized){
  MockActuator *electrovalve_null = nullptr;
  MockActuator *pump_null = nullptr;
  Irrigation_zone irrigation(electrovalve_null, pump_null);
  EXPECT_EQ(false, irrigation.is_healthy());
}