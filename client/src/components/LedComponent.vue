<script setup lang="ts">
const props = defineProps<{
  data: boolean;
  name: string;
  click: any;
}>();
</script>

<template>
  <div class="card">
    <h3>{{ props.name }}</h3>
    <div class="outer" :class="{ active: props.data }" @click="props.click">
      <div class="toggle"></div>
    </div>
  </div>
</template>

<style scoped>
h3 {
  margin-bottom: 1em;
  text-align: center;
}
</style>
