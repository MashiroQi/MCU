<template>
  <div class="wrapper">
    <div class="header-wrapper">
      <div class="header-title">
        <span>空气质量</span>
        <span>蚌埠市</span>
      </div>
      <div class="header-text">
        <span>11</span>
        <span>22</span>
      </div>
      <div class="weather-advice">12312312</div>
    </div>
    <div class="data-wrapper">
      <div class="data">
        <img class="data-logo" src="/static/images/temperature.png" />
        <div class="data-text">
          温度
          <div class="data-value">20℃</div>
        </div>
      </div>
      <div class="data">
        <img class="data-logo" src="/static/images/humirity.png" />
        <div class="data-text">
          湿度
          <div class="data-value">0%</div>
        </div>
      </div>
    </div>
    <div class="data-wrapper">
      <div class="data">
        <img class="data-logo" src="/static/images/weather.png" />
        <div class="data-text">
          天气
          <div class="data-value">阴</div>
        </div>
      </div>
      <div class="data">
        <img class="data-logo" src="/static/images/LED.png" />
        <div class="data-text">
          客厅灯
          <div class="data-value">
            <switchcomponent activeColor="#3d7ef9" inactiveColor="#e0e0ff" />
          </div>
        </div>
      </div>
    </div>
    <div class="data-wrapper">
      <div class="data">
        <img class="data-logo" src="/static/images/alarm.png" />
        <div class="data-text">
          警报
          <div class="data-value">
            <switchcomponent activeColor="#3d7ef9" inactiveColor="#e0e0ff" />
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import mqtt from "mqtt";
const options = {
  clean: false, // true: 清除会话, false: 保留会话
  connectTimeout: 4000, // 超时时间
  // 认证信息
  clientId: "emqx_test1",
};
const connectUrl = "wss://lot.qzhtest.top:8084/mqtt";
const client = mqtt.connect(connectUrl, options);
export default {
  name: "client",
  data() {
    return {
      client: {},
    };
  },
  methods: {
    MqttConnect() {
      console.log("开始连接");
      client.on("connect", function () {
        console.log("成功连接mqtt服务器!");

        client.subscribe("test", function (err) {
          if (!err) {
            console.log("成功订阅!");
          } else {
            console.log("订阅失败!");
          }
        });
      });
    },
  },

  created() {
    MqttConnect();
    client.publish("devtest", '{text:"Hello World!"}');
    console.log('发送{text:"Hello World!"}');
    client.on("message", (topic, message) => {
      console.log("收到消息：", message.toString());
    });
  },
};
</script>

<style lang="scss" scoped>
.wrapper {
  padding: 15px;
  .header-wrapper {
    background-color: #3d7ef9;
    border-radius: 20px;
    color: #fff;
    box-shadow: #d6d6d6 0px 0px 5px;
    padding: 15px 30px;
    .header-title {
      display: flex;
      justify-content: space-between;
    }
    .header-text {
      font-size: 32px;
      font-weight: 400;
      display: flex;
      justify-content: space-between;
    }
    .weather-advice {
      margin-top: 20px;
      font-size: 12px;
    }
  }
  .data-wrapper {
    margin-top: 20px;
    display: flex;
    justify-content: space-between;
    .data {
      background-color: #fff;
      width: 150px;
      height: 80px;
      border-radius: 20px;
      display: flex;
      justify-content: space-around;
      padding: 8px;
      box-shadow: #d6d6d6 0px 0px 5px;
      .data-logo {
        height: 36px;
        width: 36px;
        margin-top: 15px;
      }
      .data-text {
        margin-top: 15px;
        color: #7f7f7f;
        .data-value {
          font-size: 26px;
        }
      }
    }
  }
}
</style>

