import { defineStore } from "pinia";
import { ref } from "vue";

export interface Data {
  username: string | null;
  password: string | null;
}

export const dataStore = defineStore("dataStore", () => {
  // data
  const data = ref<Data>({
    username: null,
    password: null,
  });

  // actions

  const logIn = async (username: string, password: string) => {
    data.value.username = username;
    data.value.password = password;

    const req = await fetch(`${import.meta.env.VITE_API_URL}login`, {
      method: "POST",
      body: JSON.stringify(data.value),
    });
  };

  return { logIn };
});