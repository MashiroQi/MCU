<template>
  <div class="switchcomponent">
    <span class="switchcomponent_core">
      <span class="switchcomponent_button"></span>
    </span>
  </div>
</template>
 
<script>
export default {
  name: "switchcomponent",
  props: {},
  methods: {},
};
</script>
 
<style lang="scss" scoped>
.switchcomponent {
  display: inline-block;
  align-items: center;
  position: relative;
  font-size: 14px;
  line-height: 20px;
  vertical-align: middle;
  .switchcomponent_core {
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
    transition: border-color 0.3s, background-color 0.3s;
    vertical-align: middle;
    .switchcomponent_button {
      position: absolute;
      top: 1px;
      left: 1px;
      border-radius: 100%;
      transition: all 0.3s;
      width: 16px;
      height: 16px;
      background-color: #fff;
    }
  }
}
</style>