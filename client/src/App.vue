<script setup lang="ts">
import { onMounted, ref } from "vue";
import CommonCardComponent from "@/components/CommonCardComponent.vue";
import DHTComponentVue from "@/components/DHTComponent.vue";
import { dataStore } from "@/stores/dataStore";
import { storeToRefs } from "pinia";
import MoonIcon from "@/icons/MoonIcon.vue";
import SunIcon from "@/icons/SunIcon.vue";

const data = storeToRefs(dataStore()).data;

const theme = ref("");

onMounted(() => {
  theme.value = localStorage.getItem("theme") || "light";
  document.body.classList.add(theme.value);

  dataStore().fetchData();
});

const changeTheme = () => {
  if (theme.value === "light") {
    localStorage.setItem("theme", "dark");
    theme.value = "dark";
    document.body.classList.replace("light", "dark");
  } else {
    localStorage.setItem("theme", "light");
    theme.value = "light";
    document.body.classList.replace("dark", "light");
  }
};
</script>

<template>
  <header>
    <h1>ESP32 dashboard</h1>
    <button @click="changeTheme" class="theme-switcher">
      <MoonIcon v-if="theme === 'dark'"></MoonIcon>
      <SunIcon v-if="theme === 'light'"></SunIcon>
    </button>
  </header>

  <main>
    <CommonCardComponent
      name="Heater"
      :data="data.heater"
      :click="dataStore().toggleHeater"
      :disabled="dataStore().data.auto"
    ></CommonCardComponent>
    <CommonCardComponent
      name="Humidifier"
      :data="data.humidifier"
      :click="dataStore().toggleHumidifier"
      :disabled="dataStore().data.auto"
    ></CommonCardComponent>
    <DHTComponentVue :data="data.dht"> </DHTComponentVue>
    <CommonCardComponent
      name="Auto"
      :data="dataStore().data.auto"
      :click="dataStore().toggleAuto"
    ></CommonCardComponent>
    <div class="card">
      <h3>Temperature</h3>

      <input
        type="number"
        min="0"
        max="50"
        step="1"
        @change="dataStore().setTemperature"
      />
    </div>
    <div class="card">
      <h3>Humidity</h3>

      <input
        type="number"
        min="0"
        max="100"
        step="1"
        @change="dataStore().setHumidity"
      />
    </div>
  </main>
</template>

<style>
main {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(50px, 150px));
  justify-content: center;
  width: 100%;
  max-width: 900px;
  gap: 20px;
  align-self: center;
  margin-top: 20vh;
  padding: 1em;
}

header {
  display: flex;
  flex-direction: row;
  justify-content: space-between;
  padding: 1em;
  position: fixed;
  background: var(--background);
  top: 0;
  width: 100%;
}

header h1 {
  font-family: "Inter";
  background: var(--gradient);
  -webkit-background-clip: text;
  background-clip: text;
  font-weight: 700;
  color: transparent;
}

.theme-switcher {
  width: 60px;
  height: 50px;
  padding: 0.5em 1em !important;
}

.theme-switcher svg path {
  fill: var(--foreground);
}

.theme-switcher svg {
  width: 80%;
  min-width: 20px;
}

.card input {
  width: 100%;
  font-size: 1.1rem;
  padding: 0.5rem;
  border: 2px solid var(--accent);
  border-radius: 10px;
  outline: none;
}

.card h3 {
  margin-bottom: 1rem;
}
</style>
