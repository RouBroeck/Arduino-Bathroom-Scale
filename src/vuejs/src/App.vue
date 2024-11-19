<template>
  <nav class="navbar navbar-expand fixed-top navbar-dark bg-dark p-2">
    <a class="navbar-brand" href="">
      Bathroom-Scale
    </a>
    <ul class="navbar-nav me-auto">
      <li class="nav-item">
        <a class="nav-link" href="#current">Current</a>
      </li>
      <li class="nav-item">
        <a class="nav-link" href="#week">Week</a>
      </li>
      <li class="nav-item">
        <a class="nav-link" href="#month">Month</a>
      </li>
    </ul>
  </nav>
  <div class="d-inline-flex flex-column">
    <div class="card" style="width: 48rem;">
      <h1 id="current"> Current </h1>
      <Graph data="[10,10,10]"/>
    </div>
    <div class="card" style="width: 48rem;">
      <h1 id="week"> Week </h1>
      <Graph data="[10,10,10]"/>
    </div>
    <div class="card" style="width: 48rem;">
      <h1 id="month"> Month </h1>
      <Graph data="[10,10,10]"/>
    </div>
  </div>
</template>

<script>
import Graph from './components/Graph.vue'
export default {
  name: 'App',
  components: {
    Graph,
  }
}

</script>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 60px;
}
</style>
