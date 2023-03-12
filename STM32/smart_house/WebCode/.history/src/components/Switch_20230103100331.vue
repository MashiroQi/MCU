<template>
  <div
    class="switchcomponent"
    :class="{ 'is-checked': value }"
    @click="handleClick"
  >
    <span class="switchcomponent_core" ref="core">
      <span class="switchcomponent_button"></span>
    </span>
    <input
      type="checkbox"
      class="switchcomponent_input"
      :name="name"
      ref="input"
    />
  </div>
</template>
 
<script>
export default {
  name: "GSwitch",
  props: {
    value: {
      type: Boolean,
      default: false,
    },
    activeColor: {
      type: String,
      default: "",
    },
    inactiveColor: {
      type: String,
      default: "",
    },
    name: {
      type: String,
      default: "",
    },
  },
  methods: {
    handleClick() {
      this.$emit("input", !this.value);
      this.setColor();
    },
    setColor() {
      // 修改开关的颜色
      if (this.activeColor || this.inactiveColor) {
        const color = this.value ? this.activeColor : this.inactiveColor;
        // console.log(color)
        this.$refs.core.style.borderColor = color;
        this.$refs.core.style.backgroundColor = color;
      }
      // 控制checkbox的值，input值同步value值
      this.$refs.input.checked = this.value;
    },
  },
  mounted() {
    this.setColor();
  },
};
</script>
 
<style lang="scss" scoped>
  .switchcomponent{
    display: inline-block;
    align-items: center;
    position: relative;
    font-size: 14px;
    line-height: 20px;
    vertical-align: middle;
    .switchcomponent_core{
    margin: 0;
    display: inline-block;
    position: relative;
    width: 40px;
    height: 20px;
    border: 1px solid #dcdfe6;
    outline: none;
    border-radius: 10px;
    box-sizing: border-box;
    background: #dcdfe6;
    cursor: pointer;
    transition: border-color .3s,background-color .3s;
    vertical-align: middle;
    .switchcomponent_button{
      position:absolute;
      top: 1px;
      left: 1px;
      border-radius: 100%;
      transition: all .3s;
      width: 16px;
      height: 16px;
      background-color: #fff;
      }
    }
  }
  // 选中样式
  .is-checked {
    .switchcomponent_core{
      border-color: #409eff;
      background-color: #409eff;
      .switchcomponent_button {
        transform: translateX(20px);
      }
    }
  }
  // 隐藏input标签
  .switchcomponent_input{
    position:absolute;
    width: 0;
    height: 0;
    opacity: 0;
    margin: 0;
  }

————————————————
版权声明：本文为CSDN博主「二木成林」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/cnds123321/article/details/109332776