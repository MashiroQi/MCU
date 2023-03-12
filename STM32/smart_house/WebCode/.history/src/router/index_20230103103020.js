import Vue from 'vue'
import Router from 'vue-router'
import Client from '@/components/client'
Vue.use(Router)

export default new Router({
  routes: [
    {
      path: '/',
      name: 'client',
      component: client
    }
  ]
})
