<template>
  <div>
    <span
      class="weui-switch"
      :class="{ 'weui-switch-on': isChecked }"
      :value="value"
      @click="toggle"
      style="position: relative"
    >
      <div
        v-if="isChecked && direction.length > 0"
        style="
          width: 100%;
          height: 100%;
          position: absolute;
          padding: 0 5px;
          line-height: 20px;
          color: #fff;
          user-select: none;
        "
      >
        {{ direction[0] }}
      </div>
      <div
        v-if="!isChecked && direction.length > 0"
        style="
          width: 100%;
          height: 100%;
          position: absolute;
          padding: 0 5px;
          right: 2px;
          line-height: 22px;
          color: #7a7a7a;
          text-align: right;
          user-select: none;
        "
      >
        {{ direction[1] }}
      </div>
    </span>
  </div>
</template>
<script>
export default {
  name: "switchComponent",
  props: {
    value: {
      type: Boolean,
      default: true,
    },
    text: {
      type: String,
      default: "",
    },
  },
  data() {
    return {
      isChecked: this.value,
    };
  },
  computed: {
    direction() {
      if (this.text) {
        return this.text.split("|");
      } else {
        return [];
      }
    },
  },
  watch: {
    value(val) {
      this.isChecked = val;
    },
    isChecked(val) {
      this.$emit("change", val);
    },
  },
  methods: {
    toggle() {
      this.isChecked = !this.isChecked;
    },
  },
};
</script>
<style>
.weui-switch {
  display: block;
  position: relative;
  width: 52px;
  height: 24px;
  border: 1px solid #dfdfdf;
  outline: 0;
  border-radius: 16px;
  box-sizing: border-box;
  background-color: #dfdfdf;
  transition: background-color 0.1s, border 0.1s;
  cursor: pointer;
}
.weui-switch:before {
  content: " ";
  position: absolute;
  top: 0;
  left: 0;
  width: 50px;
  height: 22px;
  border-radius: 15px;
  background-color: #fdfdfd;
  transition: transform 0.35s cubic-bezier(0.45, 1, 0.4, 1);
}
.weui-switch:after {
  content: " ";
  position: absolute;
  top: 0;
  left: 0;
  width: 22px;
  height: 22px;
  border-radius: 15px;
  background-color: #ffffff;
  box-shadow: 0 1px 3px rgba(0, 0, 0, 0.4);
  transition: transform 0.35s cubic-bezier(0.4, 0.4, 0.25, 1.35);
}
.weui-switch-on {
  border-color: #6f6f6f;
  background-color: #1aad19;
}
.weui-switch-on:before {
  border-color: #1aad19;
  background-color: #409eff;
}
.weui-switch-on:after {
  transform: translateX(28px);
}
</style>