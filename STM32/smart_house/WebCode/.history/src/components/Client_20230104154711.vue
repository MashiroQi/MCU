<template>
  <div class="wrapper">
    <div class="header-wrapper">
      <div class="header-title">
        <!-- <span>空气质量</span> -->
        <span>{{ City }} 市</span>
      </div>
      <div class="header-text">
        <!--
        <span> {{ AirValue }} </span>
        -->
        <span>{{ TodyTemp }}℃</span>
      </div>
      <!--
      <div class="weather-advice">12312312</div>
        -->
    </div>
    <div class="data-wrapper">
      <div class="data">
        <img class="data-logo" src="/static/images/temperature.png" />
        <div class="data-text">
          温度
          <div class="data-value">{{ Temp }}℃</div>
        </div>
      </div>
      <div class="data">
        <img class="data-logo" src="/static/images/humirity.png" />
        <div class="data-text">
          湿度
          <div class="data-value">{{ Humi }}%</div>
        </div>
      </div>
    </div>
    <div class="data-wrapper">
      <div class="data">
        <img class="data-logo" src="/static/images/weather.png" />
        <div class="data-text">
          天气
          <div class="data-value">{{ Weather }}</div>
        </div>
      </div>
      <div class="data">
        <img class="data-logo" src="/static/images/LED.png" />
        <div class="data-text">
          客厅灯
          <div class="data-value">
            <switchcomponent
              @change="onLedChange"
              :aria-checked="Led"
              activeColor="#3d7ef9"
              inactiveColor="#e0e0ff"
            />
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
            <switchcomponent
              @change="onBeepChange"
              :aria-checked="Beep"
              activeColor="#3d7ef9"
              inactiveColor="#e0e0ff"
            />
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import mqtt from "mqtt";
import axios from "axios";
const options = {
  clean: true, // true: 清除会话, false: 保留会话
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
      /* AirValue: "", */
      City: "芜湖",
      TodyTemp: 0,
      Weather: "",
      runOnce: true,
      Temp: 0,
      Humi: 0,
      Led: false,
      Beep: false,
    };
  },
  methods: {
    MqttConnect() {
      console.log("开始连接");
      client.on("connect", function () {
        console.log("成功连接mqtt服务器!");

        client.subscribe("/home/pub", function (err) {
          if (!err) {
            console.log("成功订阅!");
          } else {
            console.log("订阅失败!");
          }
        });
      });
    },
    Init() {
      if (this.runOnce) {
        this.MqttConnect();
        this.runOnce = false;
      }
    },
    onLedChange(event) {
      console.log("led", event);
      let sw1 = event;
      if (sw1) {
        client.publish("/home/sub", '{ "LED_SW" : 1 }', (err) => {
          if (!err) {
            console.log("发送成功");
          }
        });
      } else {
        client.publish("/home/sub", '{ "LED_SW" : 0 }', (err) => {
          if (!err) {
            console.log("发送成功");
          }
        });
      }
    },
    onBeepChange(event) {
      console.log("beep", event);
      let sw2 = event;
      if (sw2) {
        client.publish("/home/sub", '{"BEEP_SW":1}', (err) => {
          if (!err) {
            console.log("发送成功");
          }
        });
      } else {
        client.publish("/home/sub", '{"BEEP_SW":0}', (err) => {
          if (!err) {
            console.log("发送成功");
          }
        });
      }
    },
  },

  created() {
    this.Init();
    /* client.publish("devtest", '{text:"Hello World!"}');
    console.log('devtest 发送{text:"Hello World!"}'); */
    client.on("message", (topic, message) => {
      let dataFromDev = {};
      dataFromDev = JSON.parse(message);
      this.Temp = dataFromDev.Temp;
      this.Humi = dataFromDev.Humi;

      console.log("收到消息：", dataFromDev);
    });

    axios
      .get(
        "https://api.seniverse.com/v3/weather/now.json?key=SdhaQd3OweoTJ0uzP&location=wuhu&language=zh-Hans&unit=c",
        { params: {} }
      )
      .then((res) => {
        // res.data 是服务器返回的数据
        this.Weather = res.data.results[0].now.text;
        this.TodyTemp = res.data.results[0].now.temperature;
        console.log(res.data.results[0].now);
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
        .data-title {
          text-align: right;
        }
        .data-value {
          font-size: 26px;
        }
      }
    }
  }
}
</style>

