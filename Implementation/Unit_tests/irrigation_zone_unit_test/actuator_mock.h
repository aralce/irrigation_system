//GPIO_mock.h
#include "gmock/gmock.h"  // Brings in gMock.

class Actuator
{
public:
    virtual ~Actuator() {}
    virtual void set( bool set_ON ) = 0;
    virtual bool is_ON() const = 0;
    virtual bool has_sensor() const = 0;
    virtual uint32_t read_sensor() const = 0;
};

class MockActuator: public Actuator
{
public:
    MOCK_METHOD( void, set, (bool set_ON), (override));
    MOCK_METHOD( bool, is_ON, (), (const override));
    MOCK_METHOD( bool, has_sensor, (), (const override));
    MOCK_METHOD( uint32_t, read_sensor, (), (const override));  
};