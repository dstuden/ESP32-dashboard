import { defineStore } from "pinia";
import { ref } from "vue";

export interface Data {
  led1State: boolean;
  led2State: boolean;
  dht: {
    temperature: number | null;
    humidity: number | null;
  };
}

export const dataStore = defineStore("dataStore", () => {
  // state
  const data = ref<Data>({
    led1State: false,
    led2State: false,
    dht: {
      temperature: null,
      humidity: null,
    },
  });

  // actions
  const fetchData = async () => {
    const led1 = await await fetch(`${import.meta.env.VITE_API_URL}led1`);
    const led2 = await await fetch(`${import.meta.env.VITE_API_URL}led2`);
    const dht = await await fetch(`${import.meta.env.VITE_API_URL}dht`);

    const newData: Data = {
      led1State: (await led1.json()).pinState || false,
      led2State: (await led2.json()).pinState || false,
      dht: (await dht.json()) || {
        temperature: null,
        humidity: null,
      },
    };

    data.value = newData;
  };

  const fetchLed1 = async () => {
    const newData = await fetch(`${import.meta.env.VITE_API_URL}led1`);
    data.value.led1State = (await newData.json()).pinState || false;
  };

  const fetchLed2 = async () => {
    const newData = await fetch(`${import.meta.env.VITE_API_URL}led2`);
    data.value.led2State = (await newData.json()).pinState || false;
  };

  const fetchDHT = async () => {
    const newData = await fetch(`${import.meta.env.VITE_API_URL}temp`);
    data.value.dht = (await newData.json()) || {
      temperature: null,
      humidity: null,
    };
  };

  const toggleLed1 = async () => {
    const res = await fetch(`${import.meta.env.VITE_API_URL}led1`, {
      method: "POST",
    });
    const resData = await res.json();

    if (resData) data.value.led1State = resData.pinState;
  };

  const toggleLed2 = async () => {
    const res = await fetch(`${import.meta.env.VITE_API_URL}led2`, {
      method: "POST",
    });
    const resData = await res.json();

    if (resData) data.value.led2State = resData.pinState;
  };

  return {
    data,
    fetchData,
    fetchLed1,
    fetchLed2,
    toggleLed1,
    toggleLed2,
    fetchDHT,
  };
});
