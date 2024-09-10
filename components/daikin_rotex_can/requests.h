#pragma once

#include "esphome/components/daikin_rotex_can/request.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include <vector>

namespace esphome {
namespace daikin_rotex_can {

class TRequests {
public:
    TRequests();
    void add(esphome::daikin_rotex_can::TRequest const& request);

    void removeInvalidRequests();

    void setCanbus(esphome::esp32_can::ESP32Can* pCanbus);
    esphome::esp32_can::ESP32Can* getCanbus() const;

    uint32_t size() const;
    TRequest const& get(uint32_t index) const;
    TRequest const* get(std::string const& id) const;

    EntityBase* get_entity(std::string const& id);
    sensor::Sensor* get_sensor(std::string const& id);
    text_sensor::TextSensor* get_text_sensor(std::string const& id);

    bool sendNextPendingGet();
    void sendSet(std::string const& request_name, float value);
    void handle(uint32_t can_id, std::vector<uint8_t> const& responseData);

private:
    TRequest* getNextRequestToSend();

    std::vector<TRequest> m_requests;
    esphome::esp32_can::ESP32Can* m_pCanbus;
};

inline void TRequests::setCanbus(esphome::esp32_can::ESP32Can* pCanbus) {
    m_pCanbus = pCanbus;
}

inline esphome::esp32_can::ESP32Can* TRequests::getCanbus() const {
    return m_pCanbus;
}

inline uint32_t TRequests::size() const {
    return m_requests.size();
}

inline TRequest const& TRequests::get(uint32_t index) const {
    return m_requests[index];
}

inline TRequest const* TRequests::get(std::string const& id) const {
    for (auto& request: m_requests) {
        if (request.get_id() == id) {
            return &request;
        }
    }
    return nullptr;
}

inline EntityBase* TRequests::get_entity(std::string const& id) {
    TRequest const* pRequest = get(id);
    return pRequest != nullptr ? pRequest->getEntity() : nullptr;
}

inline sensor::Sensor* TRequests::get_sensor(std::string const& id) {
    return Utils::toSensor(get_entity(id));
}

inline text_sensor::TextSensor* TRequests::get_text_sensor(std::string const& id) {
    return Utils::toTextSensor(get_entity(id));
}


}
}