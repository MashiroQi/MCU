import Vue from 'vue'
import Router from 'vue-router'
import HelloWorld from '@/components/HelloWorld'
import Asd from '@/components/Client'
Vue.use(Router)

export default new Router({
  routes: [
    {
      path: '/',
      name: 'Asd',
      component: Asd
    }
  ]
})
