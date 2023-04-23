<script setup lang="ts">
const props = defineProps<{
  data: boolean;
  name: string;
  click: any;
  disabled?: boolean;
}>();

const click = () => {
  if (!props.disabled) props.click();
};
</script>

<template>
  <div class="card">
    <h3>{{ props.name }}</h3>
    <div
      class="outer"
      :class="{ active: props.data, disabled: props.disabled }"
      @click="click"
    >
      <div class="toggle"></div>
    </div>
  </div>
</template>

<style scoped>
h3 {
  margin-bottom: 1em;
  text-align: center;
}

.disabled {
  opacity: 0.5;

  cursor: unset !important;
}
</style>
