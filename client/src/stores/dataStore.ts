import { defineStore } from "pinia";
import { ref } from "vue";

export interface Data {
  heater: boolean;
  humidifier: boolean;
  dht: {
    temperature: number | null;
    humidity: number | null;
  };
  auto: boolean;
}

export const dataStore = defineStore("dataStore", () => {
  // state
  const data = ref<Data>({
    heater: false,
    humidifier: false,
    dht: {
      temperature: null,
      humidity: null,
    },
    auto: false,
  });

  // actions
  const fetchData = async () => {
    const heater = await await fetch(`${import.meta.env.VITE_API_URL}heater`);
    const humidifier = await await fetch(
      `${import.meta.env.VITE_API_URL}humidifier`
    );
    const dht = await await fetch(`${import.meta.env.VITE_API_URL}dht`);
    const auto = await await fetch(`${import.meta.env.VITE_API_URL}auto`);

    const newData: Data = {
      heater: (await heater.json()).pinState || false,
      humidifier: (await humidifier.json()).pinState || false,
      dht: (await dht.json()) || {
        temperature: null,
        humidity: null,
      },
      auto: (await auto.json()).state || false,
    };

    data.value = newData;
  };

  const fetchHeat = async () => {
    const newData = await fetch(`${import.meta.env.VITE_API_URL}heater`);
    data.value.heater = (await newData.json()).pinState || false;
  };

  const fetchHum = async () => {
    const newData = await fetch(`${import.meta.env.VITE_API_URL}humidifier`);
    data.value.humidifier = (await newData.json()).pinState || false;
  };

  const fetchDHT = async () => {
    const newData = await fetch(`${import.meta.env.VITE_API_URL}temp`);
    data.value.dht = (await newData.json()) || {
      temperature: null,
      humidity: null,
    };
  };

  const toggleHeater = async () => {
    const res = await fetch(`${import.meta.env.VITE_API_URL}heater`, {
      method: "POST",
    });
    const resData = await res.json();

    if (resData) {
      data.value.heater = resData.pinState;
      data.value.auto = false;
    }
  };

  const toggleHumidifier = async () => {
    const res = await fetch(`${import.meta.env.VITE_API_URL}humidifier`, {
      method: "POST",
    });
    const resData = await res.json();

    if (resData) {
      data.value.humidifier = resData.pinState;
      data.value.auto = false;
    }
  };

  const toggleAuto = async () => {
    const res = await fetch(`${import.meta.env.VITE_API_URL}auto`, {
      method: "POST",
    });
    const resData = await res.json();
    if (resData) {
      data.value.auto = resData.state;
    }
    await fetchData();
  };

  const setTemperature = async (temperature: number) => {
    const res = await fetch(`${import.meta.env.VITE_API_URL}setTemp`, {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify({ temp: temperature }),
    });
  };

  const setHumidity = async (humidity: number) => {
    const res = await fetch(`${import.meta.env.VITE_API_URL}setHum`, {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify({ hum: humidity }),
    });
  };

  return {
    data,
    fetchData,
    fetchHeat,
    fetchHum,
    toggleHeater,
    toggleHumidifier,
    fetchDHT,
    toggleAuto,
    setTemperature,
    setHumidity,
  };
});
