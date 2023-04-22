<script setup lang="ts">
const props = defineProps<{
  data: {
    temperature: number | null;
    humidity: number | null;
  };
}>();
</script>

<template>
  <div class="card">
    <h3>Temperatura</h3>
    <p>
      {{ props.data.temperature?.toFixed(2) }}
      {{ props.data.temperature ? "°C" : "" }}
    </p>
  </div>
  <div class="card">
    <h3>Vlaga</h3>
    {{ props.data.humidity?.toFixed(2) }}
    {{ props.data.humidity ? "%" : "" }}
  </div>
</template>

<style scoped>
h3 {
  margin-bottom: 1em;
  text-align: center;
}
</style>
