import Vue from 'vue'
import Router from 'vue-router'
import Client from '@/components/Client'
import switchcomponent from '@/components/Switch'
Vue.use(Router)

export default new Router({
  routes: [
    {
      path: '/',
      name: 'Client',
      component: Client
    }
  ]
})
