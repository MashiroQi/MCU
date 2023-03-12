<template>
  <div
    :class="['switchcomponent', disabled ? 'is-disabled' : '']"
    :style="{
      backgroundColor: isOk ? activeColor : inactiveColor,
      transform: `scale(${ratio})`,
    }"
    @click="switchVal"
  >
    <div
      class="switchcomponent-radio"
      :style="{ left: isOk ? '1px' : '22px' }"
    ></div>
  </div>
</template>

<script>
export default {
  name: "switchcomponent",
  props: {
    ratio: {
      type: Number,
      default: 1,
    },
    activeColor: {
      type: String,
      default: "#13ce66",
    },
    inactiveColor: {
      type: String,
      default: "#ff4949",
    },
    disabled: {
      type: Boolean,
      default: false,
    },
    default: {
      type: Array,
      default: [],
    },
    value: {
      type: String | Number | Boolean,
      require: true,
    },
  },
  data() {
    return {
      isOk: null,
      rules: [],
    };
  },
  watch: {
    value: {
      handler(val) {
        // 以下方式只需要在初始化执行即可
        if (this.default.length === 2 && this.default[0] !== this.default[1]) {
          this.rules = JSON.parse(JSON.stringify(this.default));
          let index = this.rules.findIndex((item) => item === val);
          switch (index) {
            case -1:
              this.rules = [true, false];
              break;
            case 0:
              this.isOk = true;
              break;
            case 1:
              this.isOk = false;
              break;
          }
        } else {
          this.rules = [true, false];
          this.value ? (this.isOk = true) : (this.isOk = false);
        }
      },
      immediate: true,
      deep: true,
    },
  },
  methods: {
    switchVal() {
      if (this.disabled) {
        return;
      }
      this.isOk = !this.isOk;
      this.$emit("input", this.isOk ? this.rules[0] : this.rules[1]);
      this.$emit("change", this.isOk ? this.rules[0] : this.rules[1]);
    },
  },
};
</script>

<style lang="scss" scoped>
.switchcomponent {
  cursor: pointer;
  width: 40px;
  height: 20px;
  border-radius: 10px;
  position: relative;

  .switchcomponent-radio {
    position: absolute;
    top: 50%;
    left: 1px;
    transform: translate(0, -50%);
    width: 16px;
    height: 16px;
    background-color: #fff;
    border-radius: 50%;
    transition: left 0.3s;
  }
}
.is-disabled {
  opacity: 0.6;
  cursor: not-allowed;
}
</style>
