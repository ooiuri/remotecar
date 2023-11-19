/* eslint-disable @next/next/no-img-element */
import type { NextPage } from "next";
import Head from "next/head";
import Image from "next/image";
import styles from "../styles/Home.module.css";
import { KeyboardEvent, RefObject, useEffect, useRef, useState } from "react";

import InputComponent from "../components/InputComponent";
import { API_BASE_URL } from "../utils/constants";

let leftTimer: NodeJS.Timeout;
let rightTimer: NodeJS.Timeout;
let upTimer: NodeJS.Timeout;
let downTimer: NodeJS.Timeout;

import { CameraProvider } from "../contexts/CameraContext";

import * as api from "../services/apiService";

const BASE_URL = API_BASE_URL;

const Home: NextPage = () => {
  const upButton = useRef<HTMLButtonElement>(null);
  const downButton = useRef<HTMLButtonElement>(null);
  const leftButton = useRef<HTMLButtonElement>(null);
  const rightButton = useRef<HTMLButtonElement>(null);

  const baseVelocity = 220;

  function handleKeyDown(event: KeyboardEvent<Element>) {
    if (event.key === "ArrowUp") {
      handleClick(upButton, upTimer);
    }
    if (event.key === "ArrowLeft") {
      handleClick(leftButton, leftTimer);
    }
    if (event.key === "ArrowRight") {
      handleClick(rightButton, rightTimer);
    }
    if (event.key === "ArrowDown") {
      handleClick(downButton, downTimer);
    }
  }

  function setTimer(
    button: RefObject<HTMLButtonElement>,
    timer: NodeJS.Timeout
  ) {
    clearTimeout(timer);
    timer = setTimeout(() => {
      button.current?.blur();
    }, 300);
    button.current?.focus();
  }

  function handleClick(
    button: RefObject<HTMLButtonElement>,
    timer: NodeJS.Timeout
  ) {
    try {
      if (button == upButton) {
        setTimer(upButton, upTimer);
        api.request_move(baseVelocity, baseVelocity);
      }
      if (button == leftButton) {
        setTimer(leftButton, leftTimer);
        api.request_move(-baseVelocity, baseVelocity);
      }
      if (button == rightButton) {
        setTimer(rightButton, rightTimer);
        api.request_move(baseVelocity, -baseVelocity);
      }
      if (button == downButton) {
        setTimer(downButton, downTimer);
        api.request_move(-baseVelocity, -baseVelocity);
      }
    } catch (e) {
      console.log(e);
    }
  }

  useEffect(() => {
    window.addEventListener(
      "keydown",
      handleKeyDown as unknown as EventListener
    );
  }, []);

  return (
    <CameraProvider>
      <div className={styles.container} onKeyDown={handleKeyDown}>
        <Head>
          <title>Remote Camera Car</title>
        </Head>
        <Image src="/logo.svg" alt="RemoteCamera" width={348} height={173} />
        <main className={styles.main} onKeyPress={handleKeyDown}>
          <div>
            <div className={styles.keys}>
              <button
                ref={upButton}
                onClick={() => handleClick(upButton, upTimer)}
                className={styles.move}
              >
                <img
                  src="/arrow.svg"
                  width="40"
                  height="40"
                  alt="Move up"
                ></img>
              </button>
            </div>
            <div className={styles.row}>
              <div className={styles.keys}>
                <button
                  ref={leftButton}
                  onClick={() => handleClick(leftButton, leftTimer)}
                  className={styles.move}
                >
                  <img
                    src="/arrow.svg"
                    className={styles.arrowLeft}
                    width="40"
                    height="40"
                    alt="Move to the left"
                  ></img>
                </button>
              </div>
              <div className={styles.keys}>
                <button
                  ref={rightButton}
                  onClick={() => handleClick(rightButton, rightTimer)}
                  className={styles.move}
                >
                  <img
                    src="/arrow.svg"
                    className={styles.arrowRight}
                    width="40"
                    height="40"
                    alt="Move to the right"
                  ></img>
                </button>
              </div>
            </div>
            <div className={styles.keys}>
              <button
                ref={downButton}
                onClick={() => handleClick(downButton, downTimer)}
                className={styles.move}
              >
                <img
                  src="/arrow.svg"
                  className={styles.arrowDown}
                  width="40"
                  height="40"
                  alt="Move down"
                ></img>
              </button>
            </div>
            <InputComponent />
          </div>
          <div>
            <img
              src="http://192.168.100.111:8090/?action=stream"
              width={480}
              height={"auto"}
            />
          </div>
        </main>

        {/* <footer className={styles.footer}>
          Criado por <a href="https://github.com/ooiuri">ooiuri</a>! 🤍
        </footer>  */}
      </div>
    </CameraProvider>
  );
};

export default Home;
